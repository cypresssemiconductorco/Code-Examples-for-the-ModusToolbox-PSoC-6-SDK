/******************************************************************************
* File Name: uart_debug.c
*
* Version: 1.0
*
* Description: This file contains the task that provides thread-safe debug
*              message printing
*
* Related Document: CE223468_Interfacing_BMI160_FreeRTOS.pdf
*
* Hardware Dependency: See CE223468_Interfacing_BMI160_FreeRTOS.pdf
*
*******************************************************************************
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/
/* Header file includes */
#include "uart_debug.h"

#if (UART_DEBUG_ENABLE)

/* Queue handle for debug message Queue */       
QueueHandle_t debugMessageQ;

cy_stc_scb_uart_context_t KIT_UART_context;
    
/*******************************************************************************
* Function Name: void Task_Debug(void *pvParameters)
********************************************************************************
* Summary:
*  Task that prints a debug message via UART STDIO
*
* Parameters:
*  void *pvParameters : Task parameter defined during task creation (unused)                            
*
* Return:
*  void
*
*******************************************************************************/    
void Task_Debug(void *pvParameters)
{
    /* Variable that stores the data to be printed */
    debug_print_data_t dataToPrint;
    
   /* Variable used to store the return values of RTOS APIs */
    BaseType_t rtosApiResult;
    
    /* Remove warning for unused parameter */
    (void) pvParameters;
    
    /* Repeatedly running part of the task */    
    for(;;)
    {
    	/* Block until a message to printed has been received over
		   debugMessageQ */
		rtosApiResult = xQueueReceive(debugMessageQ, &dataToPrint,
									  portMAX_DELAY);

		/* Message has been received from debugMessageQ */
		if(rtosApiResult == pdTRUE)
		{
			/* If the error code is not 0, print message string along with the
			   error code (as 32-bit hexadecimal data) */
			if(dataToPrint.errorCode != 0u)
			{
				DebugPrintf("%s %"PRIX32" \r\n", dataToPrint.stringPointer,
							dataToPrint.errorCode);
			}
			/* Otherwise, print the message string only */
			else
			{
				DebugPrintf("%s \r\n", dataToPrint.stringPointer);
			}
		}
    }
}

/*******************************************************************************
* Function Name: void DEBUG_UART_FastStart(void)
********************************************************************************
*
* Summary:
*  UART initialization routine
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void DEBUG_UART_FastStart(void)
{
    Cy_SCB_UART_Init(KIT_UART_HW, &KIT_UART_config, &KIT_UART_context);
    Cy_SCB_UART_Enable(KIT_UART_HW);
}

#endif /* UART_DEBUG_ENABLE */ 

/* [] END OF FILE */
