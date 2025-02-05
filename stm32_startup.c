/*
 stm32_startup.c:

 MCU: STM32F407G-DISC1

 Flow:
 A) Vector Table
    - Contains initial addresses of code memory.
        - Addresses of all supported interrupts. (82 IRQs)
        - Address of all supported system exceptions. (15 ARM coretex M4 system exceptions)
            - 82+15+1 word addressable memory locations.
            - 392 Bytes.
        - see data sheet for specifics.
        - for vector table reserved spaced place 0.
    - stored in user defined section.
 B) System Exception and Interrupt Definitions
    - write a single default handler for all exceptions.
        - this allows the programmer to implement required handler based on application requirements.
        - uses an alias attribute:
            - when a function is defined with an alias attribute the compiler knows when it sees this function to refer to another function (the alias).
    - write necessary system exception and interrupt handlers:
        - programmer will overide handlers as defined in the vector table.
        - attribute weak: makes overiding possible.
C) Initialize .data and .bss section in SRAM:
    - Done in the reset handler.
 */
#include <stdint.h>

#define SRAM_START  0X20000000U
#define SRAM_SIZE   (128 * 1024)    // SRAM1 + SRAM2 Kbytes
#define SRAM_END    ( (SRAM_START) + (SRAM_SIZE) )

#define STACK_START SRAM_END

extern uint32_t _end_of_text;
extern uint32_t _start_of_data;
extern uint32_t _end_of_data;
extern uint32_t _load_address_of_data;
extern uint32_t _start_of_bss;
extern uint32_t _end_of_bss;

void __libc_init_array(void);
int main(void);

/* 
function prototypes of STM32F407x system exception and IRQ handlers 
obtained from: https://github.com/niekiran/CortexMxProgramming/blob/master/Source_code/mcu_exception_handlr_prototypes
*/

void Reset_Handler                  (void);
void NMI_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler 			(void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler 					(void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler   				(void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler  				(void) __attribute__ ((weak, alias("Default_Handler")));
void WWDG_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void TAMP_STAMP_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));      
void RTC_WKUP_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));                               
void RCC_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void EXTI0_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI1_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI3_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void EXTI4_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));           
void DMA1_Stream0_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream1_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream2_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream3_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream4_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream5_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void DMA1_Stream6_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));    
void ADC_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));             
void CAN1_TX_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void CAN1_RX0_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void CAN1_RX1_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void CAN1_SCE_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));        
void EXTI9_5_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void TIM1_BRK_TIM9_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));   
void TIM1_UP_TIM10_IRQHandler 		(void) __attribute__ ((weak, alias("Default_Handler")));   
void TIM1_TRG_COM_TIM11_IRQHandler 	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void TIM2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void TIM3_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void TIM4_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void I2C1_EV_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C1_ER_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C2_EV_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void I2C2_ER_IRQHandler 			(void) __attribute__ ((weak, alias("Default_Handler")));         
void SPI1_IRQHandler  				(void) __attribute__ ((weak, alias("Default_Handler")));           
void SPI2_IRQHandler 				(void) __attribute__ ((weak, alias("Default_Handler")));            
void USART1_IRQHandler  			(void) __attribute__ ((weak, alias("Default_Handler")));         
void USART2_IRQHandler  			(void) __attribute__ ((weak, alias("Default_Handler")));        
void USART3_IRQHandler   			(void) __attribute__ ((weak, alias("Default_Handler")));        
void EXTI15_10_IRQHandler   		(void) __attribute__ ((weak, alias("Default_Handler")));     
void RTC_Alarm_IRQHandler    		(void) __attribute__ ((weak, alias("Default_Handler")));    
void OTG_FS_WKUP_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler"))); 
void TIM8_BRK_TIM12_IRQHandler   	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler    	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler 	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void FSMC_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void SDIO_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler            	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void ETH_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));
void ETH_WKUP_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler     	(void) __attribute__ ((weak, alias("Default_Handler")));
void USART6_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler          	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler   	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler    	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler      	(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler           	(void) __attribute__ ((weak, alias("Default_Handler")));
void DCMI_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void CRYP_IRQHandler             	(void) __attribute__ ((weak, alias("Default_Handler")));
void HASH_RNG_IRQHandler         	(void) __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler              	(void) __attribute__ ((weak, alias("Default_Handler")));

/*
attribute section: tells the compiler to place this data in the user defined section .isr_vector rather than .data or .bss
    - run arm-none-eabi-objdump -h stm32_startup.o after make to see this newly defined section.
*/
uint32_t vectors[] __attribute__ ((section (".isr_vector"))) = {
    STACK_START,                                
    (uint32_t)Reset_Handler,
    (uint32_t)NMI_Handler, 					
    (uint32_t)HardFault_Handler, 				
    (uint32_t)MemManage_Handler, 				
    (uint32_t)BusFault_Handler, 				
    (uint32_t)UsageFault_Handler,
    0,
    0,
    0,
    0,			
    (uint32_t)SVC_Handler, 					
    (uint32_t)DebugMon_Handler,
    0, 				
    (uint32_t)PendSV_Handler,   				
    (uint32_t)SysTick_Handler,  				
    (uint32_t)WWDG_IRQHandler, 				
    (uint32_t)PVD_IRQHandler, 				             
    (uint32_t)TAMP_STAMP_IRQHandler, 			      
    (uint32_t)RTC_WKUP_IRQHandler, 
    0,			                               
    (uint32_t)RCC_IRQHandler, 				             
    (uint32_t)EXTI0_IRQHandler, 				           
    (uint32_t)EXTI1_IRQHandler, 				           
    (uint32_t)EXTI2_IRQHandler, 				           
    (uint32_t)EXTI3_IRQHandler, 				           
    (uint32_t)EXTI4_IRQHandler, 				           
    (uint32_t)DMA1_Stream0_IRQHandler, 		    
    (uint32_t)DMA1_Stream1_IRQHandler, 		    
    (uint32_t)DMA1_Stream2_IRQHandler, 		    
    (uint32_t)DMA1_Stream3_IRQHandler, 		    
    (uint32_t)DMA1_Stream4_IRQHandler, 		    
    (uint32_t)DMA1_Stream5_IRQHandler, 		    
    (uint32_t)DMA1_Stream6_IRQHandler, 		    
    (uint32_t)ADC_IRQHandler, 				             
    (uint32_t)CAN1_TX_IRQHandler, 			         
    (uint32_t)CAN1_RX0_IRQHandler, 			        
    (uint32_t)CAN1_RX1_IRQHandler, 			        
    (uint32_t)CAN1_SCE_IRQHandler, 			        
    (uint32_t)EXTI9_5_IRQHandler, 			         
    (uint32_t)TIM1_BRK_TIM9_IRQHandler, 		   
    (uint32_t)TIM1_UP_TIM10_IRQHandler, 		   
    (uint32_t)TIM1_TRG_COM_TIM11_IRQHandler, 	
    (uint32_t)TIM1_CC_IRQHandler, 			         
    (uint32_t)TIM2_IRQHandler, 				            
    (uint32_t)TIM3_IRQHandler, 				            
    (uint32_t)TIM4_IRQHandler, 				            
    (uint32_t)I2C1_EV_IRQHandler, 			         
    (uint32_t)I2C1_ER_IRQHandler, 			         
    (uint32_t)I2C2_EV_IRQHandler, 			         
    (uint32_t)I2C2_ER_IRQHandler, 			         
    (uint32_t)SPI1_IRQHandler,  				           
    (uint32_t)SPI2_IRQHandler, 				            
    (uint32_t)USART1_IRQHandler,  			         
    (uint32_t)USART2_IRQHandler,  			        
    (uint32_t)USART3_IRQHandler,   			        
    (uint32_t)EXTI15_10_IRQHandler,   		     
    (uint32_t)RTC_Alarm_IRQHandler,    		    
    (uint32_t)OTG_FS_WKUP_IRQHandler,     	 
    (uint32_t)TIM8_BRK_TIM12_IRQHandler,   	
    (uint32_t)TIM8_UP_TIM13_IRQHandler,    	
    (uint32_t)TIM8_TRG_COM_TIM14_IRQHandler, 	
    (uint32_t)TIM8_CC_IRQHandler,          	
    (uint32_t)DMA1_Stream7_IRQHandler,     	
    (uint32_t)FSMC_IRQHandler,             	
    (uint32_t)SDIO_IRQHandler,             	
    (uint32_t)TIM5_IRQHandler,             	
    (uint32_t)SPI3_IRQHandler,             	
    (uint32_t)UART4_IRQHandler,            	
    (uint32_t)UART5_IRQHandler,            	
    (uint32_t)TIM6_DAC_IRQHandler,         	
    (uint32_t)TIM7_IRQHandler,             	
    (uint32_t)DMA2_Stream0_IRQHandler,     	
    (uint32_t)DMA2_Stream1_IRQHandler,     	
    (uint32_t)DMA2_Stream2_IRQHandler,     	
    (uint32_t)DMA2_Stream3_IRQHandler,     	
    (uint32_t)DMA2_Stream4_IRQHandler,     	
    (uint32_t)ETH_IRQHandler,              	
    (uint32_t)ETH_WKUP_IRQHandler,         	
    (uint32_t)CAN2_TX_IRQHandler,          	
    (uint32_t)CAN2_RX0_IRQHandler,         	
    (uint32_t)CAN2_RX1_IRQHandler,         	
    (uint32_t)CAN2_SCE_IRQHandler,         	
    (uint32_t)OTG_FS_IRQHandler,           	
    (uint32_t)DMA2_Stream5_IRQHandler,     	
    (uint32_t)DMA2_Stream6_IRQHandler,     	
    (uint32_t)DMA2_Stream7_IRQHandler,     	
    (uint32_t)USART6_IRQHandler,           	
    (uint32_t)I2C3_EV_IRQHandler,          	
    (uint32_t)I2C3_ER_IRQHandler,          	
    (uint32_t)OTG_HS_EP1_OUT_IRQHandler,   	
    (uint32_t)OTG_HS_EP1_IN_IRQHandler,    	
    (uint32_t)OTG_HS_WKUP_IRQHandler,      	
    (uint32_t)OTG_HS_IRQHandler,           	
    (uint32_t)DCMI_IRQHandler,             	
    (uint32_t)CRYP_IRQHandler,             	
    (uint32_t)HASH_RNG_IRQHandler,         	
    (uint32_t)FPU_IRQHandler
}; 


/*
Reset_Handler
    1. copy .data section from FLASH to SRAM. .data and .rodata 
    2. Init. .bss section to zero.
    3. call init function of std. lib.
    4. call main.
*/
void Reset_Handler(void) 
{
    // 1. a) get size of data section
    uint32_t size = (uint32_t)&_end_of_data - (uint32_t)&_start_of_data;

    // 1.b) initialize destination and source pointers (byte specific)
    uint8_t *pDestination = (uint8_t*)&_start_of_data;          // SRAM
    uint8_t *pSource = (uint8_t*)&_load_address_of_data;                 // FLASH

    // 1.c) copy from source to destination for the size of data section
    for(uint32_t i = 0; i < size; i++ )
    {
            *pDestination++ = *pSource++;
    }

    // 2.a) get size of bss section
    size = (uint32_t)&_end_of_bss - (uint32_t)&_start_of_bss;

    // 2.b) initialize destination and source pointers (byte specific)
    pDestination = (uint8_t*)&_start_of_bss;

    // 2.c) copy 0 to the .bss section
    for(uint32_t i = 0; i < size; i++ )
    {
            *pDestination++ = 0;
    }

    // 3. call init function of std. lib.
    __libc_init_array();

    // 4. call main.
    main();
};

void Default_Handler(void) 
{
    while(1);
};

