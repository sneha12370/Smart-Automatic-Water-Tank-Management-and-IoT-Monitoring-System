typedef unsigned int u32;
typedef unsigned char u8;
typedef signed int s32;
typedef signed char s8;

extern void config_vic_for_eint0(void);	
extern void EINT0_Handler(void) __irq;

extern void config_vic_for_eint1(void);
extern void EINT1_Handler(void) __irq;
//extern unsigned int c1;
//extern char flag;
extern void adc_init(void);
extern unsigned int adc_read(unsigned char);

extern void uart0_init(unsigned int);
extern void uart0_tx(unsigned char);
extern unsigned char uart0_rx(void);	
extern void uart0_tx_string(char *);
extern void uart0_integer(int);
extern void uart0_float(float);

extern void delay_sec(unsigned int);
extern void delay_ms(unsigned int);

extern void lcd_cmd(unsigned char); 
extern void lcd_data (unsigned char );
extern void lcd_init(void);
extern void lcd_string(char *);
extern void lcd_float(float );
extern void lcd_integer(int );

extern void adc_init(void);
extern	unsigned int adc_read(unsigned char);

extern void ultra_delay(unsigned int); 
extern void ultra_init(void);
extern unsigned int ultra_dist(void);

	extern int temp_sensor(void);

extern	unsigned int water_sensor(void);

extern void servo_motor_init(void);
	extern void servo_0deg(void);
extern void servo_180deg(void);
	extern void servo_90deg(void);

extern void i2c_init(void);
extern void i2c_write(u8 , u8 , u8 );
	extern u8 i2c_read(u8 ,u8 );

extern void eeprom(void);
extern void wifi(int, int);


