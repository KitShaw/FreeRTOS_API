
#include "stm32f10x.h"
#include "misc.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
//#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "delay.h"
#include "stm32f10x_tim.h"

#define START_TASK_PRIO		1		     //�������ȼ�
#define START_STK_SIZE 		256				//�����ջ��С
TaskHandle_t StartTask_Handler;			//������
void start_task(void *pvParameters);//������


#define ECHO_TASK_PRIO 2   //�������ȼ�
#define ECHO_STK_SIZE 256	//�����ջ
TaskHandle_t echoTask_Handler;       //������
void echo_task(void *pv);    //������

#define TASK1_TASK_PRIO  2//�������ȼ�
#define TASK1_STK_SIZE	128 //�����ջ��С
TaskHandle_t Task1Task_Handler;   //������
void task1_task(void *pvParameters); //������

//�������ȼ�
#define QUERY_TASK_PRIO		3
//�����ջ��С	
#define QUERY_STK_SIZE 		256
//������
TaskHandle_t QueryTask_Handler;
//������
void query_task(void *pvParameters);

char InfoBuffer[1000];          //������Ϣ������



int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�ж����ȼ����� ����4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //������
							 (const char*   )"start_task",  //��������
							 (uint16_t      )START_STK_SIZE, //�����ջ��С
							 (void*         )NULL, 						//������������Ĳ���
							 (UBaseType_t   )START_TASK_PRIO, //�������ȼ�
							 (TaskHandle_t* )&StartTask_Handler); //������
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//��ʼ����������
{	
	taskENTER_CRITICAL();           //�����ٽ���
	
	xTaskCreate((TaskFunction_t   )echo_task,
							(const char*      )"echo_task",
							(uint16_t         )ECHO_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )ECHO_TASK_PRIO,
							(TaskHandle_t*    )&echoTask_Handler);
							
	    xTaskCreate((TaskFunction_t )query_task,     
                (const char*    )"query_task",   
                (uint16_t       )QUERY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )QUERY_TASK_PRIO,
                (TaskHandle_t*  )&QueryTask_Handler); 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����						
	vTaskDelete(StartTask_Handler);   //ɾ����ʼ����
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //������
{
	char c1;
	while(1)
	{
		//printf("%s", scanf(""));
		c1 = fgetc(stdin);
		//if(c1 == 'j') printf("k\r\n");
		switch(c1)
		{
			case '4':
				fputc('4',stdout);
			break;
			case '3':
				fputc('3',stdout);
			break;
			case '2':

			break;
			case '1':

			break;
		}
	}
}


//query������
void query_task(void *pvParameters)
{	
	while(fgetc(stdin) == 'i') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('i',stdout);
	
	while(1)
	{
		//LED1=!LED1;
		printf("Hello!\r\n");
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}







