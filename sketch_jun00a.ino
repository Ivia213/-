/*#include <Servo.h>                           // Arduino 伺服舵机库文件
  Servo servo;                                 // 创建舵机对象
  servo.attach(int pin,[int min],[int max]);   // 舵机控制,支持~PWM的GPIO针脚 ,最小角度,最大角度
  servo.detach();                              // 使舵机与其针脚分离，该针脚可继续被用作PWM接口
  servo.write(int value);                      // 如果值为< 200，则视为角度，否则为脉冲宽度（微秒）
  servo.writeMicroseconds(int value);          // 以微秒为单位写入脉冲宽度
  servo.read();                                //<int>返回最后一次write( )命令中的角度值
  servo.readMicroseconds();                    //<int>返回伺服的最后一次脉冲宽度
  servo.attached();                            //<bool>判断舵机参数是否已发送到舵机所在的接口
//-----------------------------------------------
  1000为完全逆时针方向，2000完全顺时针方向，1500为在中间。舵机通常响应在500到2500之间的值

   0.5ms-------------0度；
   1.0ms------------45度；
   1.5ms------------90度；
   2.0ms-----------135度；
   2.5ms-----------180度；
   SG90 无负载速度：0.12秒/60度(4.8V) = 120毫秒60度 = 2毫秒1度 = 2000微秒1度
   MG90 无负载速度：0.11秒/60度(4.8V)

   注意:电脑USB2.0和USB3.0都输出5V 标准电流为500ma和900ma电流.
   建议 开发板插电脑 USB3.0进行操作.
   SG90 工作电压4.8V~6V  待机5ma,工作电流300ma..堵转扭矩：1.2-1.4公斤/厘米(4.8V)
   MG90S工作电压4.8V~6V  待机8ma,工作电流300ma..堵转扭矩: 1.8-2.0公斤/厘米(4.8V)
   
//--------------------------------------------
   360度舵机与180度舵机不同,180度舵机是给一个特定的PWM信号,舵机会转到相应的角度.
   360度舵机不能控制角度旋转,只能够控制方向和旋转转速,所以360度舵机给定一个PWM,会以特定的速度和方向转动。
   PWM信号与360舵机转速的关系：
   0.5ms----------------正向最大转速；
   1.5ms----------------速度为0；
   2.5ms----------------反向最大转速；

淘宝店铺 创客与编程            https://shop104415339.taobao.com/
4轴机械臂 https://item.taobao.com/item.htm?ft=t&id=634846120104
5轴机械臂 https://item.taobao.com/item.htm?ft=t&id=638328575007
6轴机械臂 https://item.taobao.com/item.htm?ft=t&id=637592400765
TIANKONGRC舵机 https://item.taobao.com/item.htm?ft=t&id=637949946243
*/

#include <Servo.h>          //引用舵机库文件
Servo servo;                //创建舵机对象

//-----------------------------------------------------------------------
//
//
//
//------------------开发板通电后初始函数 setup()----------------------------
void setup() {  //setup 在开发板通电后最先被调用一次.这里可以初始化一些功能或数据
    Serial.begin(115200);                   //初始化串口 波特率115200或9600
    Serial.println("setup"); 
    
    pinMode(LED_BUILTIN , OUTPUT);          //设置开发板 LED灯为输出
    digitalWrite(LED_BUILTIN,HIGH);         //Uno板 HIGH=亮,LOW=灭


    //---------------------舵机控制代码-----------------------------------
    servo.attach( 9);                        //arduino uno开发板 舵机对象绑定 GPIO 9号针脚
    //servo.attach(D8);                        //esp8266 开发板 舵机对象绑定 D8=GPIO ?号针脚
    
    for(int i=0;i<=180;i++){                //角度方式控制舵机从0度转到180度
      for(int j=10;j>0;j--){                //每度循环发送10次信号
        servo.write(i);                     //舵机对象 发送角度值
        delay(1);                           //适量延时等待舵机转到目标角度
      }
    }
    for(int i=180;i>0;i--){                 //再次让舵机从180度转到0度
      for(int j=10;j>0;j--){
        servo.write(i);
        delay(1);
      }
    }

    for(int j=360;j>0;j--){                 //舵机性能参数120毫秒转60度,360毫秒转180度
       servo.write(90);                     //直接指定90度,让舵机快速转过来...
       delay(1);                            //延时1毫秒,循环360次,舵机不管在哪都能转到指定角度
    }
/*
  //用高低电平脉冲微秒时间宽度方式控制舵机.比角度更精确平滑.每1角度约等于11.11微秒脉宽
    for(int i=500;i<=2500;i++){            //500脉宽=0度,1500=90度,2500=180度
      for(int j=1;j>0;j--){
        servo.writeMicroseconds(i);        //舵机对象 发送脉冲微秒值
        delay(1);
      }
    }
    for(int i=2500;i>500;i--){             //从180度转回到0度
      for(int j=1;j>0;j--){
        servo.writeMicroseconds(i);
        delay(1);
      }
    }
    for(int j=360;j>0;j--){
      servo.writeMicroseconds(1500);      //直接指定1500脉宽=90度,让舵机快速转过来...
      delay(1);
    }
*/

  //设置数字针脚 GPIO 7,GPIO 8 输入按键信号....供 loop  检测按键开关用..
    pinMode(7,INPUT_PULLUP);        //若按钮按下时输入高电平则用 INPUT=输入
    pinMode(8,INPUT_PULLUP);        //若按钮按下时输入低电平则用 INPUT_PULLUP=输入上拉

    Serial.println("setup.end");
}




//-----------------------------------------------------------------
// 双轴按键摇杆 PS2游戏摇杆控制杆传感器JoyStick. 针脚向下时:X上加下减,Y左减右加..针脚向右时:X左加右减,Y上加下减..
//   接5V电居中时:X=481|482,Y=518   接3.3V居中时:X=323|327,Y:339|341
//   VRX,接模拟口.
//   VRY,接模拟口.
//    SW,接数字口.轻按输入=1,弹起=0,按到底=0
//------------------开发板循环调用函数 loop()------------------------
void loop() {                   //loop 开发板会一直循环调用这个loop...
  int old=servo.read();         //取回最后一次给舵机的角度值
  
  //Serial.print("old:");
  //Serial.println(old, DEC);   //串口输出之前写入舵机的角度值

  //---------------------双轴摇杆控制舵机转向-----------------
/*
  int X=analogRead(0);                  //读取A0模拟值..先把摇杆信号线插到A0接口
  Serial.print("X:");
  Serial.println(X, DEC);
  if(X<200 && old>0){                   //摇杆向左..3.3V居中时:X=323|327,Y:339|341
    for(int j=5;j>0;j--){               //循环发5次重复命令为了让舵机不出现遗漏.
       servo.write(old-1);              //左转1度
       delay(1);                        //舵机性能参数120毫秒转60度,每度0.5毫秒
    }
  }if(X>500 && old<180){                //摇杆向右..3.3V居中时:X=323|327,Y:339|341
    for(int j=5;j>0;j--){               //循环发5次重复命令为了让舵机不出现遗漏.
       servo.write(old+1);              //向右1度
       delay(1);                        //延时1毫秒,循环了5次足够转10度
    }
  }
*/

  //---------------------检测数字 GPIO 7,GPIO 8有无按钮开关输入低电平信号----------
/*
  Serial.print("GPIO 7:");
  Serial.print(digitalRead(7), DEC);
  Serial.print("  ,GPIO 8:");
  Serial.println(digitalRead(8), DEC);
  if(!digitalRead(7) && old>0){          //7号针脚输入低电平,摇杆向左.若判断高电平,删掉!感叹号即可
    for(int j=5;j>0;j--){                //循环发5次重复命令为了让舵机不出现遗漏.
       servo.write(old-1);               //左转1度
       delay(1);                         //舵机性能参数120毫秒转60度,每度0.5毫秒
    }
  }if(!digitalRead(8) && old<180){       //8号针脚输入低电平,摇杆向右.若判断高电平,删掉!感叹号即可
    for(int j=5;j>0;j--){                //循环发5次重复命令为了让舵机不出现遗漏.
       servo.write(old+1);               //向右1度
       delay(1);                         //延时1毫秒,循环了5次足够转10度
    }
  }
*/


   //---------------监听电脑串口输入指令-------------------
  if(Serial.available()>0){                   //有指令数据>0字节
    String t=Serial.readString();             //? X
    Command(t);                               //解析拼执行命令
  }
}



//------------------------------------------------------------------------
//
//
//
//-----------------解析拼执行命令 已定义常用命令 ? X----------------
void Command(String t)  {              
    int old=servo.read();                   //取回最后一次给舵机的角度值


    t.trim();                               //删首尾空格换行
    t.toUpperCase();                        //指令转为大写
    Serial.println(t);                      //显口输出指令

    //----------------------- ? X --------------------------------
    if('?'==t.charAt(0)){                           //提取指令第1个字符进行判断
        Serial.println("X++ 执行后X电机角度+1度");    //Y++ Z++ E++
        Serial.println("X-- 执行后X电机角度-1度");    //Y-- Z-- E--
        Serial.println("X+? 执行后X电机相对+N度");    //Y+1 Z+3 E+8
        Serial.println("X-? 执行后X电机相对-N度");    //Y-2 Z-5 E-9
        Serial.println("X?? 执行后X电机绝对到N度");   //Y10 Z30 E90
    } else if('X'==t.charAt(0)){              //解析第一个字符为X指令
        int   v=0;                            //v变量保存新角度值
        if(t.charAt(2)=='-'){                 //X--    第3个字符
          v=old-1;                            //新角度=旧角度-1度
        } else if (t.charAt(2)=='+'){         //X++    第3个字符
          v=old+1;                            //新角度=旧角度+1度
        } else if (t.charAt(1)=='-'){         //X-?    第2个字符
          t=t.substring(2);                   //提取第3个字符与后面的数字内容
          v=old-t.toInt();                    //新角度=旧角度-N度
        } else if (t.charAt(1)=='+'){         //X+?    第2个字符
          t=t.substring(2);                   //提取第3个字符与后面的数字内容
          v=old+t.toInt();                    //新角度=旧角度+N度
        } else {                              //X???   绝对定位
          t=t.substring(1);                   //提取第2个字符与后面的数字内容
          v=t.toInt();                        //新角度=数字内容值
        }


       for(int j=abs(old-v)/2;j>0;j--){      //循环原先角度到目标角度的差,再除2,因为每度0.5毫秒即够
          if(old<v){
            old++;                           //+1度
          } else if(old>v) {
            old--;                           //-1度
          }
          servo.write(old);                  //让舵机1度1度平滑转到目标角度
          delay(1);                          //舵机性能参数120毫秒转60度,每度0.5毫秒
       }
       
       for(int j=5;j>0;j--){                 //最后再循环发送5次指令,进行角度绝对定位
          servo.write(v);                    //
          delay(1);                          //舵机性能参数120毫秒转60度,每度0.5毫秒
       }
    }
}
