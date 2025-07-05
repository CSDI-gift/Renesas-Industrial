/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-10     Sherman       first version
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <rtdbg.h>
#include <errno.h>
#include "lv_label.h"
#include <strings.h>
//#include <serial_v2.h>
#define LED_PIN    BSP_IO_PORT_02_PIN_09 /* Onboard LED pins */

extern char La16[];
extern char La18[];
extern char La19[];
extern char La20[];
extern char La11[];
extern char La22[];
extern char La21[];

extern uint8_t mode_flag;
char datas[100]={0};//数据缓冲区
uint8_t i=0;//数据指针

extern lv_obj_t * ui_Label16;
extern lv_obj_t * ui_Label18;
extern lv_obj_t * ui_Label11;
extern lv_obj_t * ui_Label19;
extern lv_obj_t * ui_Label20;
extern lv_obj_t * ui_Label21;
extern lv_obj_t * ui_Label22;

rt_device_t u4_dev;
struct rt_semaphore sem;
rt_thread_t u4_th;
struct serial_configure u4_configs = RT_SERIAL_CONFIG_DEFAULT;

rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&sem);
    return RT_EOK;
}

void serial_thread_entry(void *parameter)
{
    char buffer;
    while(1){
        while(rt_device_read(u4_dev, 0, &buffer, 1) != 1){
            rt_sem_take(&sem, RT_WAITING_FOREVER);
        }
        datas[i]=buffer;
        i++;
        if(buffer=='\0')
            i=0;

    }
}
void hal_entry(void)
{
    rt_err_t ret = 0;
    u4_dev = rt_device_find("uart4");

    if(u4_dev == RT_NULL){
        LOG_E("rt_device_find[uart4] failed...\n");
//        return -EINVAL;
    }

    ret = rt_device_open(u4_dev, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
    if(ret < 0){
        LOG_E("rt_device_open[uart4] failed...\n");
//        return ret;
    }

    rt_device_control(u4_dev, RT_DEVICE_CTRL_CONFIG, (void *)&u4_configs);

    rt_device_set_rx_indicate(u4_dev, rx_callback);

    rt_sem_init(&sem,"rx_sem", 0, RT_IPC_FLAG_FIFO);

    u4_th = rt_thread_create("u4_recv",serial_thread_entry , NULL, 1024, 10, 5);

    rt_thread_startup(u4_th);
//    rt_device_write(u4_dev, 0, "Uart4 config...\n", rt_strlen("Uart4 config...\n"));

    while (1)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
        if(i==0)
        {
            if(mode_flag==1)
            {
                uint8_t j=0;
                for(;j<100;j++)
                {
                    if(datas[j] == '\n')
                    {
                        j++;
                        lv_label_set_text(ui_Label16, La16);
                        memset(La16,0,rt_strlen(La16));
                        break;
                    }
                    La16[j]=datas[j];


                }
                for(;j<100;j++)
                {
                    static uint8_t n=0;
                    if(datas[j] == '\n')
                    {
                        lv_label_set_text(ui_Label18, La18);
                        memset(La18,0,rt_strlen(La18));
                        n=0;
                        memset(datas,0,100);
                        break;

                    }
                    La18[n]=datas[j];
                    n++;
                }

            }
            if(mode_flag==2)
            {
                uint8_t j=0;
                for(;j<100;j++)
                {
                    if(datas[j] == '\n')
                    {
                        j++;
                        lv_label_set_text(ui_Label19, La19);
                        memset(La19,0,rt_strlen(La19));
                        break;
                    }
                    La19[j]=datas[j];


                }
                for(;j<100;j++)
                {
                    static uint8_t a=0;
                    if(datas[j] == '\n')
                    {   j++;
                        lv_label_set_text(ui_Label20, La20);
                        memset(La20,0,rt_strlen(La20));
                        a=0;
                        break;

                    }
                    La20[a]=datas[j];
                    a++;
                }
                for(;j<100;j++)
                {
                    static uint8_t b=0;
                    if(datas[j] == '\n')
                    {   j++;
                        lv_label_set_text(ui_Label11, La11);
                        memset(La11,0,rt_strlen(La11));
                        b=0;
                        break;

                    }
                    La11[b]=datas[j];
                    b++;
                }
                for(;j<100;j++)
                {
                    static uint8_t c=0;
                    if(datas[j] == '\n')
                    {   j++;
                        lv_label_set_text(ui_Label22, La22);
                        memset(La22,0,rt_strlen(La22));
                        c=0;
                        break;

                    }
                    La22[j]=datas[j];
                }
                for(;j<100;j++)
                {
                    static uint8_t d=0;
                    if(datas[j] == '\n')
                    {
                        lv_label_set_text(ui_Label21, La21);
                        memset(La21,0,rt_strlen(La21));
                        d=0;
                        memset(datas,0,100);
                        break;

                    }
                    La21[d]=datas[j];
                    d++;
                }
            }
        }
    }
}
