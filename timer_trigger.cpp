#include "task.h"

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line11);
	}

}
//static uint16_t trigger_cnt=0;
//static uint8_t mode=0, status=0, i=0, j=0, k=0;

//uint8_t len;
//uint8_t ptr[4];

//void pulse_delay()
//{
//	uint16_t t=300;
//	while (t--) __nop();
//}

//void TRIGGER()
//{
////	GPIO_WriteBit(GPx_FPGA_EN, Pin_FPGA_EN, (BitAction)1);

//	GPIO_ResetBits(GPx_LED_RD, Pin_LED_RD);
//	
//	GPIO_SetBits(GPx_FPGA_TRG, Pin_FPGA_TRG);
//	pulse_delay();
//	GPIO_ResetBits(GPx_FPGA_TRG, Pin_FPGA_TRG);
//	
//	osTimerStart(id_tmr_wup, 500);

////	GPIO_WriteBit(GPx_FPGA_EN, Pin_FPGA_EN, (BitAction)0);
//}

void timer_Trigger  (void const *arg)
{
//	status = ctrlb->status;
//	
//	if (status == CB_WAIT)
//	{
//		setCTRLB(CB_TRIG);
//		
//		ctrlb->progress = 0;
//		ctrlb->times_cntdown = ctrlb->times;
//		ctrlb->interval_cntdown = STORGE_TIME;

//		disp->updated |= LU_CSTAT | LU_CVAL;
//		
//		osTimerStart(id_tmr_trg, 1000);
//	}
//	else if (status == CB_PAU)
//	{
//		osTimerStart(id_tmr_trg, 1000);
//	}
//	else if (status == CB_TRIG)
//	{
//		ctrlb->interval_cntdown--;
//		if (ctrlb->interval_cntdown == 0)
//		{
//			TRIGGER();
//			ctrlb->times_cntdown--;
//			
//			if (ctrlb->times_cntdown != 0 || ctrlb->times == 0)
//			{
//				ctrlb->times_cntdown = ctrlb->times ? ctrlb->times_cntdown : ctrlb->times_cntdown+2;
//				
//				ctrlb->progress = ctrlb->times ? ((float)(ctrlb->times-ctrlb->times_cntdown)*100.0/ctrlb->times) : 0;
//				ctrlb->interval_cntdown = ctrlb->interval;
//				osTimerStart(id_tmr_trg, 1000);
//			}
//			else
//			{
//				ctrlb->times_cntdown = 0;
//				ctrlb->interval_cntdown = 0;
//				ctrlb->progress = 100;
//				GPIO_SetBits(GPx_LED_YL, Pin_LED_YL);
//				
//				setCTRLB(CB_WAIT);
//				
//				disp->mode = 0;
//				disp->updated = 0xff;
//			}
//		}
//		else
//		{
//			osTimerStart(id_tmr_trg, 1000);
//		}
//		
//		disp->updated |= LU_CVAL;
//	}
}

//void timer_wakeup (void const *arg)
//{
//	GPIO_SetBits(GPx_LED_RD, Pin_LED_RD);
//	
//	if (ctrlb->status != CB_CHK && ctrlb->status != CB_FIN)
//	{
//		i=0;
//		while(src[i]!=NULL)
//		{
//			src[i]->global_updated=0x03;
//			for (k=0; k<src[i]->stage; k++)
//				src[i]->stage_updated[k]=0x03;
//			i++;
//		}
//	}
//}
