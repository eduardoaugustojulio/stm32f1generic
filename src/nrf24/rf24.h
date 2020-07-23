typedef struct 
{
  	uint8_t ce_pin;
  	uint8_t csn_pin;
  	
	uint16_t spi_speed;
  	
	uint8_t spi_rxbuff[32+1];
  	uint8_t spi_txbuff[32+1]; 
  
	uint8_t payload_size;

 	uint8_t addr_width;
}rf24_t;
  
typedef enum 
{
	PA_MIN = 0,
	PA_LOW, 
	PA_HIGH, 
	PA_MAX, 
	PA_ERROR 
} rf24_pa_dbm_e ;

typedef enum 
{ 
	_250KBPS = 0, 
	_1MBPS, 
	_2MPS 
} rf24_datarate_e;

typedef enum 
{ 
	DISABLED = 0, 
	CRC_8, 
	CRC_16 
} rf24_crclength_e;

void rf24_init(uint8_t _cepin, uint8_t _cspin, uint32_t _spi_speed);
