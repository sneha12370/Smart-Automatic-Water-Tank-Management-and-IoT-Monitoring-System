#include<lpc21xx.h>
#include"header.h"
#include<stdio.h>
#include<string.h>

// Fill these in before use
#define WIFI_SSID mhhsml
#define WIFI_PASS password123
#define THINGSPEAK_KEY YJYEGVPAFI6UOISO

// UART1: TXD1=P0.8, RXD1=P0.9 (separate from UART0 used for debug)
#define U1THRE  ((U1LSR>>5)&1)
#define U1RDR   (U1LSR&1)

static void uart1_init(unsigned int baud)
{
	int pclk, result;
	if(VPBDIV == 0x0)   
	   pclk = 15000000;
	else if(VPBDIV == 0x1) 
	pclk = 60000000;
	else                 
	  pclk = 30000000;
	result = pclk / (16 * baud);
	PINSEL0 |= (1<<16)|(1<<18);  // P0.8=TXD1, P0.9=RXD1
	U1LCR = 0x83;
	U1DLL = result & 0xff;
	U1DLM = (result >> 8) & 0xff;
	U1LCR = 0x03;
}

static void uart1_tx(unsigned char data)
{
	while(U1THRE == 0);
	U1THR = data;
}

static void uart1_tx_string(char *ptr)
{
	while(*ptr != 0) { uart1_tx(*ptr++); }
}

// Read UART1 response, return 1 if "OK" found, 0 if "ERROR" or timeout
static int uart1_check_ok(int timeout_ms)
{
	char buf[64];
	int i = 0, elapsed = 0;
	while(elapsed < timeout_ms && i < 63)
	{
		if(U1RDR)
		{
			buf[i++] = U1RBR;
			buf[i] = '\0';
			if(strstr(buf, "OK"))    return 1;
			if(strstr(buf, "ERROR")) return 0;
		}
		else
		{
			delay_ms(1);
			elapsed++;
		}
	}
	return 0;
}

// Wait for '>' prompt that ESP8266 sends after AT+CIPSEND=N
static int uart1_wait_prompt(int timeout_ms)
{
	int elapsed = 0;
	while(elapsed < timeout_ms)
	{
		if(U1RDR)
		{
			if(U1RBR == '>') return 1;
		}
		else
		{
			delay_ms(1);
			elapsed++;
		}
	}
	return 0;
}

void wifi(int wl, int temper)
{
	char http_req[128];
	char cmd[32];
	int req_len;
	static int initialized = 0;

	if(!initialized)
	{
		uart1_init(9600);
		initialized = 1;
	}

	// Test AT
	uart1_tx_string("AT\r\n");
	if(!uart1_check_ok(1000))
	{
		uart0_tx_string("WiFi: AT failed\r\n");
		return;
	}

	// Set station mode
	uart1_tx_string("AT+CWMODE=1\r\n");
	if(!uart1_check_ok(1000))
	{
		uart0_tx_string("WiFi: CWMODE failed\r\n");
		return;
	}

	// Connect to WiFi
	uart0_tx_string("WiFi: Connecting...\r\n");
	uart1_tx_string("AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASS "\"\r\n");
	if(!uart1_check_ok(10000))
	{
		uart0_tx_string("WiFi: Connection failed\r\n");
		return;
	}
	uart0_tx_string("WiFi: Connected\r\n");

	// Start TCP connection to ThingSpeak
	uart1_tx_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	if(!uart1_check_ok(5000))
	{
		uart0_tx_string("WiFi: TCP connect failed\r\n");
		return;
	}

	// Build HTTP GET with actual sensor values
	sprintf(http_req,
		"GET /update?api_key=" THINGSPEAK_KEY "&field1=%d&field2=%d HTTP/1.1\r\n"
		"Host: api.thingspeak.com\r\n"
		"Connection: close\r\n\r\n",
		wl, temper);
	req_len = strlen(http_req);

	// Tell ESP8266 exact byte count
	sprintf(cmd, "AT+CIPSEND=%d\r\n", req_len);
	uart1_tx_string(cmd);
	if(!uart1_wait_prompt(2000))
	{
		uart0_tx_string("WiFi: CIPSEND failed\r\n");
		return;
	}

	// Send HTTP request
	uart1_tx_string(http_req);
	if(!uart1_check_ok(5000))
	{
		uart0_tx_string("WiFi: Send failed\r\n");
		return;
	}
	uart0_tx_string("WiFi: Data sent\r\n");

	// Close TCP connection
	uart1_tx_string("AT+CIPCLOSE\r\n");
	uart1_check_ok(2000);
}
