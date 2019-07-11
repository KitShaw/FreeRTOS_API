
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
#define LIST_TASK_PRIO		3
//任务堆栈大小	
#define LIST_STK_SIZE 		128  
//任务句柄
TaskHandle_t ListTask_Handler;
//任务函数
void list_task(void *pvParameters);


//定义一个测试用的列表和3个列表项
List_t TestList;  //测试用列表
ListItem_t ListItem1;  //测试用列表项1
ListItem_t ListItem2;
ListItem_t ListItem3;


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
							
	    xTaskCreate((TaskFunction_t )list_task,     
                (const char*    )"list_task",   
                (uint16_t       )LIST_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LIST_TASK_PRIO,
                (TaskHandle_t*  )&ListTask_Handler); 
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


//list任务函数
void list_task(void *pvParameters)
{
	//第一步：初始化列表和列表项
	vListInitialise(&TestList);
	vListInitialiseItem(&ListItem1);
	vListInitialiseItem(&ListItem2);
	vListInitialiseItem(&ListItem3);
	
	ListItem1.xItemValue=40;			//ListItem1列表项值为40
	ListItem2.xItemValue=60;			//ListItem2列表项值为60
	ListItem3.xItemValue=50;			//ListItem3列表项值为50
	
	//第二步：打印列表和其他列表项的地址
	printf("/*******************列表和列表项地址*******************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->xListEnd.xItemValue     %#x					\r\n",(int)TestList.xListEnd.xItemValue);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_i键继续!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'i') delay_ms(10);					//等待KEY_UP键按下
	fputc('i',stdout);
	//第三步：向列表TestList添加列表项ListItem1，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem1);		//插入列表项ListItem1
	printf("/******************添加列表项ListItem1*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_j键继续!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'j') delay_ms(10);					//等待KEY_UP键按下
	fputc('j',stdout);
	//第四步：向列表TestList添加列表项ListItem2，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem2);	//插入列表项ListItem2
	printf("/******************添加列表项ListItem2*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_k键继续!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'k') delay_ms(10);					//等待KEY_UP键按下
	fputc('k',stdout);
	//第五步：向列表TestList添加列表项ListItem3，并通过串口打印所有
	//列表项中成员变量pxNext和pxPrevious的值，通过这两个值观察列表
	//项在列表中的连接情况。
	vListInsert(&TestList,&ListItem3);	//插入列表项ListItem3
	printf("/******************添加列表项ListItem3*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_l键继续!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'l') delay_ms(10);					//等待KEY_UP键按下
	fputc('l',stdout);
	//第六步：删除ListItem2，并通过串口打印所有列表项中成员变量pxNext和
	//pxPrevious的值，通过这两个值观察列表项在列表中的连接情况。
	uxListRemove(&ListItem2);						//删除ListItem2
	printf("/******************删除列表项ListItem2*****************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************结束**************************/\r\n");
	printf("按下KEY_m键继续!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'm') delay_ms(10);					//等待KEY_UP键按下
	fputc('m',stdout);
	//第七步：删除ListItem2，并通过串口打印所有列表项中成员变量pxNext和
	//pxPrevious的值，通过这两个值观察列表项在列表中的连接情况。
	TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex向后移一项，这样pxIndex就会指向ListItem1。
	vListInsertEnd(&TestList,&ListItem2);				//列表末尾添加列表项ListItem2
	printf("/***************在末尾添加列表项ListItem2***************/\r\n");
	printf("项目                              地址				    \r\n");
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************前后向连接分割线********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************结束**************************/\r\n\r\n\r\n");
	while(1)
	{
		//LED1=!LED1;
		printf("Hello!\r\n");
        vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
	}
}







