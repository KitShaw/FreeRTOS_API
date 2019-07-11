
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

#define START_TASK_PRIO		1		     //任务优先级
#define START_STK_SIZE 		256				//任务堆栈大小
TaskHandle_t StartTask_Handler;			//任务句柄
void start_task(void *pvParameters);//任务函数


#define ECHO_TASK_PRIO 2   //任务优先级
#define ECHO_STK_SIZE 256	//任务堆栈
TaskHandle_t echoTask_Handler;       //任务句柄
void echo_task(void *pv);    //任务函数

#define TASK1_TASK_PRIO  2//任务优先级
#define TASK1_STK_SIZE	128 //任务堆栈大小
TaskHandle_t Task1Task_Handler;   //任务句柄
void task1_task(void *pvParameters); //任务函数

//任务优先级
#define QUERY_TASK_PRIO		3
//任务堆栈大小	
#define QUERY_STK_SIZE 		256
//任务句柄
TaskHandle_t QueryTask_Handler;
//任务函数
void query_task(void *pvParameters);

char InfoBuffer[1000];          //保持信息的数组



int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分组4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //任务函数
							 (const char*   )"start_task",  //任务名称
							 (uint16_t      )START_STK_SIZE, //任务堆栈大小
							 (void*         )NULL, 						//传达给任务函数的参数
							 (UBaseType_t   )START_TASK_PRIO, //任务优先级
							 (TaskHandle_t* )&StartTask_Handler); //任务句柄
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//开始任务任务函数
{	
	taskENTER_CRITICAL();           //进入临界区
	
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
    vTaskDelete(StartTask_Handler); //删除开始任务						
	vTaskDelete(StartTask_Handler);   //删除开始任务
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //任务函数
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


//query任务函数
void query_task(void *pvParameters)
{	
	u32 TotalRunTime;
	UBaseType_t ArraySize,x;
	TaskStatus_t *StatusArray;
	
	//第一步：函数uxTaskGetSystemState()的使用
	printf("/********第一步：函数uxTaskGetSystemState()的使用**********/\r\n");
	ArraySize=uxTaskGetNumberOfTasks();		//获取系统任务数量
	StatusArray=pvPortMalloc(ArraySize*sizeof(TaskStatus_t));//申请内存
	if(StatusArray!=NULL)					//内存申请成功
	{
		ArraySize=uxTaskGetSystemState((TaskStatus_t* 	)StatusArray, 	//任务信息存储数组
									   (UBaseType_t		)ArraySize, 	//任务信息存储数组大小
								       (uint32_t*		)&TotalRunTime);//保存系统总的运行时间
		printf("TaskName\t\tPriority\t\tTaskNumber\t\t\r\n");
		for(x=0;x<ArraySize;x++)
		{
			//通过串口打印出获取到的系统任务的有关信息，比如任务名称、
			//任务优先级和任务编号。
			printf("%s\t\t%d\t\t\t%d\t\t\t\r\n",				
					StatusArray[x].pcTaskName,
					(int)StatusArray[x].uxCurrentPriority,
					(int)StatusArray[x].xTaskNumber);
			
		}
	}	


	vPortFree(StatusArray);	//释放内存
	printf("/**************************结束***************************/\r\n");
	printf("按下KEY_UP键继续!\r\n\r\n\r\n");
	//while(KEY_Scan(0)!=WKUP_PRES) delay_ms(10);		//等待KEY_UP键按下

	while(fgetc(stdin) == 'i') delay_ms(10);					//等待KEY_UP键按下
	fputc('i',stdout);
	
	
	while(fgetc(stdin) == 'i') delay_ms(10);					//等待KEY_UP键按下
	fputc('i',stdout);

	while(1)
	{
		//LED1=!LED1;
		printf("Hello!\r\n");
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}







