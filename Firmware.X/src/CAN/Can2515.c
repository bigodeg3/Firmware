#include "Can2515.h"
#include <can2510.h>
#include <delays.h>
#include <p18f4550.h>
#include <spi.h>
#include "spi_can.h"

// M�todo de configura��o do controlador CAN externo

void configCan2515(void)
{

BYTE result;
	result = 0x04;
	result = CAN2510Init(

//Buffer Config
	CAN2510_RESET &
	CAN2510_RXB0_NOFILT &
	CAN2510_RXB1_NOFILT &
	CAN2510_RXB0_ROLL &
	CAN2510_RX1BF_OFF &
	CAN2510_RX0BF_OFF &
	CAN2510_REQ_CONFIG &
	CAN2510_CLKOUT_1, 

//BitTimeConfig
//		CAN2510_BRG_4x & 		//(BRP = 5)
//        CAN2510_SJW_1TQ &		//SJW = 1*TQ
//		CAN2510_PH2SEG_6TQ &	//PH2SEG = 6*TQ
//		CAN2510_PH1SEG_7TQ &	//PH1SEG = 7*TQ
//		CAN2510_PROPSEG_2TQ & 	//PROPSEG = 2*TQ
//		CAN2510_PH2SOURCE_PH2 & //Source is PH2
//		CAN2510_SAMPLE_1x,		//Bit sampled once

	CAN2510_BRG_2x &      // BRP = 2
	CAN2510_SJW_1TQ &     //SJW = 1*TQ
	CAN2510_PH2SEG_3TQ &  //PH2SEG = 6*TQ
	CAN2510_PH1SEG_3TQ &  //PH1SEG = 7*TQ
	CAN2510_PROPSEG_1TQ & //PROPSEG = 2*TQ
	CAN2510_PH2SOURCE_PH2 &
	CAN2510_SAMPLE_1x ,



//interruptEnables
	CAN2510_NONE_EN ,

//SPI_syncMode
	CAN2510_SPI_FOSC4 ,

//SPI_busMode
	CAN2510_SPI_MODE00, //mcp2515?

//SPI_smpPhase
	CAN2510_SPI_SMPMID	
	);
	
	Delay10KTCYx(100); 	

/*
BRP = 5
FOSC = 20
TQ = 2 * (BRP + 1) / FOSC = 2*6/20000000 = 0,6uS
NBT = TQ(1+3+3+1) = 8*TQ = 4,8uS
1/NBT = 208333
208Kbps
*/

/*
BRP = 3
TQ = 2 * (BRP + 1) / FOSC = 2*4/20000000 = 0,4uS
NBT = TQ(1+2+2+1) = 6*TQ = 2,4us
1/NBT = 0,416666Mbps => 416Kbps
*/

/*
BRP = 2
TQ = 2 * (BRP + 1) / FOSC = 2*3/20000000 = 0,3uS
NBT = TQ(1+2+2+1) = 6*TQ = 1,8us
1/NBT = 0,55555Mbps => 555Kbps
*/


// Defini��o de m�scaras e filtros do n�
	CAN2510SetMode(CAN2510_MODE_CONFIG);
	CAN2510InterruptEnable( CAN2510_NONE_EN );

	CAN2510SetSingleMaskStd( CAN2510_RXM0, 0xFF );
//	CAN2510SetSingleMaskStd( CAN2510_RXM1, 0xFF );
//	CAN2510SetSingleMaskXtd( CAN2510_RXM0, 0xFF );
//	CAN2510SetSingleMaskXtd( CAN2510_RXM1, 0xFF );

	CAN2510SetSingleFilterStd( CAN2510_RXF0, 0x02 );
//	CAN2510SetSingleFilterStd( CAN2510_RXF1, 0x02 );
//	CAN2510SetSingleFilterStd( CAN2510_RXF2, 0x02 );
//	CAN2510SetSingleFilterStd( CAN2510_RXF3, 0x02 );
//	CAN2510SetSingleFilterStd( CAN2510_RXF4, 0x02 );
//	CAN2510SetSingleFilterStd( CAN2510_RXF5, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF0, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF1, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF2, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF3, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF4, 0x02 );
//	CAN2510SetSingleFilterXtd( CAN2510_RXF5, 0x02 );
//
//


	if(result == 0x00)
	{
		PORTAbits.RA0=1; //Acende a porta RA0 se a configura��o foi feita
	}
	
	else
	{	
		PORTAbits.RA1=1; //Acende a porta RA1 se a configura��o n�o foi feita
	}
	Delay10KTCYx(100); 
	
	CAN2510SetMode(CAN2510_MODE_LOOPBACK);
	
}// Fim do m�todo de configura��o


// M�todo de envio de dados (mensagens) para o barramento CAN
void txCAN(BYTE dataReceived1, BYTE dataReceived2)
{
	BYTE data[4];
	BYTE i,r;
	r = 0x04;	

	//zerar o vetor de dados
	for(i=0;i<4;i++)
	{
		data[i] = 0x00;
	}

	data[0] = dataReceived1; //A
	data[1] = dataReceived2; //B


	r = CAN2510WriteStd(0x02, 				//id
					CAN2510_PRI_HIGHEST,	//prioridade 
					0x04, 					//Tamanho em bytes.
					data);			
	
}

// M�todo de envio de dados (de controle) para o barramento CAN
void tx_CAN(BYTE dataReceived)
{
	BYTE data[4];
	BYTE i,r;
	r = 0x04;	

	//zerar o vetor de dados
	for(i=0;i<4;i++)
	{
		data[i] = 0x00;
	}

	data[0] = dataReceived; //A

	r = CAN2510WriteStd(0x02, 			//id
                            CAN2510_PRI_HIGHEST,	//prioridade
                            0x04, 			//Tamanho em bytes.
                            data);
	
}



// M�todo de recebimento de dados do barramento CAN
unsigned char rxCAN(int index){
	auto unsigned long id;
	BYTE data[4];
	BYTE lenght;
	BYTE r;

	switch(CAN2510DataReady(CAN2510_RXBX)) //checando os dois RX buffers (RXB0 e RXB1)
	{	
		case 0: //N�o h� dado
			r = 'n'; //return;
			//return 0x18;
                        return 0x6e;
		break;
		case 1: //Existe dado no RXB0
			r = CAN2510DataRead(CAN2510_RXB0, &id, &lenght, data);			
				
			return data[index];
					
		break;
		case 2: //Existe dado no RXB1
			r = CAN2510DataRead(CAN2510_RXB1, &id, &lenght, data);

			return data[index];
						
		break;
		case 3: //Existe dado no RXB0 e RXB1
			r = CAN2510DataRead(CAN2510_RXB0, &id, &lenght, data);

			CAN2510DataRead(CAN2510_RXB1, &id, &lenght, data);
							
			return data[index];
			
		break;
	}


}

void blink_led_D2 (void){

	PORTDbits.RD2=1; //Liga led
	Delay10KTCYx(100); 
	Delay10KTCYx(100); 
	PORTDbits.RD2=0; //Desliga led
	Delay10KTCYx(100);
	Delay10KTCYx(100); 
	PORTDbits.RD2=1; //LIGA ED
	Delay10KTCYx(100);
	Delay10KTCYx(100);
	PORTDbits.RD2=0; //LIGA ED
	Delay10KTCYx(100);
	Delay10KTCYx(100);
		
}

void brilha_led_D3 (void){

	PORTDbits.RD3=1; //Liga led
	Delay10KTCYx(100); 
	Delay10KTCYx(100); 
	PORTDbits.RD3=0; //Desliga led
	Delay10KTCYx(100);
	Delay10KTCYx(100); 
	PORTDbits.RD3=1; //LIGA ED
	Delay10KTCYx(100);
	Delay10KTCYx(100);
	PORTDbits.RD3=0; //LIGA ED
	Delay10KTCYx(100);
	Delay10KTCYx(100);
		
}	

