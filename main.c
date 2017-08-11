#include "stm32f10x.h"


#define INST_WRITE					(3)
#define MAXNUM_TXPARAM			(150)
#define ID									(2)
#define LENGTH							(3)
#define INSTRUCTION					(4)
#define ERRBIT							(4)
#define PARAMETER						(5)
#define DEFAULT_BAUDNUMBER	(1)
#define MAXNUM_RXPARAM		(60)

unsigned char dxlPacket[MAXNUM_TXPARAM+10] = {0};
unsigned char gbStatusPacket[MAXNUM_RXPARAM+10] = {0};
unsigned char gbRxPacketLength = 0;
unsigned char gbRxGetLength = 0;
int giBusUsing = 0;


int dxl_initialize( int devIndex, int baudnum );
void dxl_tx_packet(void);
void dxl_txrx_packet(void);
int dxl_get_lowbyte( int word );
int dxl_get_highbyte( int word );
void dxl_write_word( int id, int address, int value );
int dxl_hal_tx( unsigned char *pPacket, int numPacket );
int dxl_hal_open( int devIndex, int baudrate );


int dxl_initialize( int devIndex, int baudnum )
{
	int baudrate;
	baudrate = 2000000 / (baudnum + 1);
	if( dxl_hal_open(devIndex, baudrate) == 0 )
		return 0;
	giBusUsing = 0;
	return 1;
}

void dxl_tx_packet()
{
	unsigned char i;
	unsigned char TxNumByte, RealTxNumByte;
	unsigned char checksum = 0;
	if( giBusUsing == 1 )
		return;
	giBusUsing = 1;
	if( dxlPacket[LENGTH] > (MAXNUM_TXPARAM+2) )
	{
		giBusUsing = 0;
		return;
	}
	if(dxlPacket[INSTRUCTION] != INST_WRITE)
	{
		giBusUsing = 0;
		return;
	}
	dxlPacket[0] = 0xff;
	dxlPacket[1] = 0xff;
	for( i=0; i<(dxlPacket[LENGTH]+1); i++ )
		checksum += dxlPacket[i+2];
	dxlPacket[dxlPacket[LENGTH]+3] = ~checksum;

	TxNumByte = dxlPacket[LENGTH] + 4;
	RealTxNumByte = dxl_hal_tx( (unsigned char*)dxlPacket, TxNumByte );
	if( TxNumByte != RealTxNumByte )
	{
		giBusUsing = 0;
		return;
	}
}

void dxl_txrx_packet()
{
	dxl_tx_packet();
}
int dxl_get_lowbyte( int word )
{
	unsigned short temp;
	temp = word & 0xff;
	return (int)temp;
}

int dxl_get_highbyte( int word )
{
	unsigned short temp;
	temp = word & 0xff00;
	temp = temp >> 8;
	return (int)temp;
}
void dxl_write_word( int id, int address, int value )
{
	while(giBusUsing);

	dxlPacket[ID] = (unsigned char)id;
	dxlPacket[INSTRUCTION] = INST_WRITE;
	dxlPacket[PARAMETER] = (unsigned char)address;
	dxlPacket[PARAMETER+1] = (unsigned char)dxl_get_lowbyte(value);
	dxlPacket[PARAMETER+2] = (unsigned char)dxl_get_highbyte(value);
	dxlPacket[LENGTH] = 5;
	dxl_txrx_packet();
}
int dxl_hal_open( int devIndex, int baudrate )
{
	USART1_Configuration(baudrate);
	return 1;
}

int dxl_hal_tx( unsigned char *pPacket, int numPacket )
{
	unsigned char i;
	for(i=0 ; i<numPacket; i++  )
		TxDByte_DXL(pPacket[i]);

	return numPacket;
}
