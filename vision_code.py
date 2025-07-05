# Edge Impulse - OpenMV Image Classification Example
#
# This work is licensed under the MIT license.
# Copyright (c) 2013-2024 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE

import sensor, time, tf, uos, gc
import lcd
from pyb import UART

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))       # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.
sensor.set_auto_whitebal(False)
sensor.set_hmirror(True)
sensor.set_vflip(True)
lcd.init()

uart = UART(2, 115200)
mode = 0 #确定程序运行
obj_threshold   = (48, 72, -22, -1, 35, 69)

#距离=k/直径像素
#实际大小=k1(k2)*直径像素
k = 20 * 160.5
k1 = 10 / 166 #5.2  3.6
k2 = 10 / 166

net = None
labels = None

def data_receive():
    if uart.any():
        global mode
        data=uart.readline().strip()
        if data==b'1':
            mode=1
            print(mode)
        elif data==b'2':
            mode=2
            print(mode)
        elif data==b'0':
            mode=0
            print(mode)

try:
    # load the model, alloc the model file on the heap if we have at least 64K free after loading
    net = tf.load("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
    print(e)
    raise Exception('Failed to load "trained.tflite", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

try:
    labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
    raise Exception('Failed to load "labels.txt", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

clock = time.clock()
while(True):
    clock.tick()

    img = sensor.snapshot()
    img = img.lens_corr(0.9)
    data_receive()
    if mode == 1:#缺陷检测
        # 关键修改：使用tf.classify替换ml.predict[6,8](@ref)
        # 通过参数调整禁用多尺度检测（纯分类模式）
        for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.5, x_overlap=0, y_overlap=0):
            # 获取分类结果（obj.output()直接返回概率数组）
            predictions = list(zip(labels, obj.output()))
            # 输出置信度最高的类别[3](@ref)
            max_index = predictions.index(max(predictions, key=lambda x:x[1]))
            if predictions[max_index][1] > 0.5 :
                print("最高置信度类别:", labels[max_index], "得分:", predictions[max_index][1])
                uart.write(labels[max_index])
                uart.write("\n")
                uart.write("%.2f"%predictions[max_index][1])
                uart.write("\n")
                uart.write("\0")
                img.draw_string(0,0,labels[max_index],scale=2)
                img.draw_string(0,20,"%.2f"%predictions[max_index][1],scale=2)

    elif mode == 2:#尺寸位置检测
        blobs = img.find_blobs([obj_threshold])
        if len(blobs) == 1:
            # Draw a rect around the blob.
            b = blobs[0]
            img.draw_rectangle(b[0:4]) # rect
            img.draw_cross(b[5], b[6]) # cx, cy
            lm1 = b[2]
            lm2 = b[3]
            Lm = (b[2]+b[3])/2
            length = k/Lm
            l1 = k1 * lm1
            l2 = k2 * lm2
            print(f"距离：{length}，长度：{l1:.2f}, 宽度：{l2:.2f}，x:{b[5]},y{b[6]}")
            
            uart.write('%.2f'%length)
            uart.write("\n")
            uart.write('%.2f'%l1)
            uart.write("\n")
            uart.write('%.2f'%l2)
            uart.write("\n")
            uart.write(str(b[5]))
            uart.write("\n")
            uart.write(str(b[6]))
            uart.write("\n")
            uart.write("\0")
    lcd.display(img)
