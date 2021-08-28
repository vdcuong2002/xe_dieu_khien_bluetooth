//ĐÂY LÀ PHẦN CODE CHO MẠCH ARDUINO CHO CHIẾC XE TỰ CHẾ CỦA MÌNH, PHẦN CODE CÓ THỂ CÒN RẤT LÀ
//LINH TINH, KHÔNG RA ĐÂU VÀO ĐÂU, CÁC HÀM CÒN THỪA THÃI HOẶC CHƯA ĐƯỢC TỐI ƯU CHO LẮM NHƯNG
//XE CHẠY HOÀN TOÀN OKE (CÓ THỂ CÓ MỘT SỐ TRỤC TRẶC TRONG QUÁ TRÌNH CHẠY) NÊN MONG CAC BẠN
//BỚT GẠCH ĐÁ GIÚP MÌNH Ạ :(((
//                                            TÁC GIẢ: VŨ ĐỨC CƯỜNG


#include<SoftwareSerial.h>                        //THƯ VIỆN GIAO TIẾP BLUETOOTH
//Chan TX va RX của Arduino kết nối ngược lại với TX, RX của Module
#define TX 3                                      //chan RX tren module cam vao so 3 tren Arduino
#define RX 2                                      //chan TX tren module cam vao so 2 tren Arduino
//KHAI BÁO CÁC HẲNG SỐ GẮN VỚI ĐẦU VÀO TRÊN MODULE TỪ 1 ĐỀN 4 ĐỂ ĐIỀU KHIỂN 4 CHÂN ĐỘNG CƠ
//Chú ý phải khai các chân PWM để set được speed đầu vào
#define IN1 5                                    //Chân PWM
#define IN2 6                                    //Chân PWM
#define IN3 9                                    //Chân PWM
#define IN4 10                                    //Chân PWM
//Khai báo tốc độ max và min của động cơ analogWrite 0->255
#define Max 255
#define Min 0
//Khai báo 3 chân hồng ngoại đọc line
#define IRL 4
#define IRR 8
//Khai báo chân chạy còi và đèn
#define ledr 13
int ledl;
#define beep 11                                  //Chân PWM
//KHAI BÁO ĐO KHOẢNG CÁCH ĐỂ CHẠY
                                //Chân nhận xung


SoftwareSerial bluetooth(RX, TX);                //TX cua Bluetooth la RX cua Arduino,, va nguoc lai


//CÁC TRẠNG THÁI
int i = 0, s = 0;
boolean mode = 0;                                //Trạng thái chuyển các mode chế độ
boolean LightStatus = 0;                         //Trạng thái của đèn soi
boolean iStatus = 0;                             //Trạng thái của quay vòng phải
boolean jStatus = 0;                             //Trạng thái của quay vòng trái
boolean IRLStatus;                               //Trạng thái ir1
boolean IRRStatus;                               //Trạng thái ir3

int baudRate[] = {300, 1200, 2400, 4800,
                  9600, 19200, 38400, 57600,
                  115200
                 };                              //Khai báo mảng để giao tiếp với Bluethooth
char kytu;                                       //Tín hiệu gửi từ app trên MitAppEnverter gửi về chuỗi ký tự ....
//Ký tự d thì đi thẳng
//Ký tự q thì đi thẳng với vận tốc cao nhất
//Ký tự u thì đi lùi
//Ký tự s là dừng
//Ký tự r là rẽ phải
//Ký tự l là rẽ trái
//Ký tự i là đi vòng bên phải
//Ký tự j là đi vòng bên trái
//Ký tự m là chuyển chế độ
//Ký tự k là bật đèn
//Ký tự b là bật còi
//Ký tự e là lùi trái
//Ký tự w là lùi phải
char lenh[2] = "ss";


//-----------------------------------------------------------------------
void setup() {
  //IN1 VÀ IN2 ĐIỀU KHIỂN 2 ĐỘNG CƠ BÊN PHẢI
  pinMode(IN1, OUTPUT);                           //Cấu hình đầu ra IN1
  pinMode(IN2, OUTPUT);                           //Cấu hình đầu ra IN2
  //IN3 VÀ IN4 ĐIỀU KHIỂN 2 ĐỘNG CƠ BÊN TRÁI
  pinMode(IN3, OUTPUT);                           //Cấu hình đầu ra IN3
  pinMode(IN4, OUTPUT);                           //Cấu hình đầu ra IN4
  //CHÂN XI NHAN
  pinMode(ledr, OUTPUT);
  pinMode(ledl, OUTPUT);
  //CHÂN CÒI
  pinMode(beep, OUTPUT);
  //ĐẦU VÀO CHÂN IR
  pinMode(IRL, INPUT);
  pinMode(IRR, INPUT);


  //CẤU HÌNH CHO MODULE BLUETOOTH
  Serial.begin(9600);                             //Khai báo Serial
  while (!Serial) {}
  Serial.println("Configuring, please wait...");
  for (int i = 0;  i < 9; i++) {
    bluetooth.begin(baudRate[i]);
    String cmd = "AT+BAUD4";                      //Giao tiếp với module bluetooth qua Serial
    bluetooth.print(cmd);
    bluetooth.flush();
    delay (100);
  }
  bluetooth.begin(9600);
  Serial.println("Config done");
  while (!bluetooth) {}
  Serial.println("Enter AT commands:");


  //-----------------------------------------------------------------------
  dung();



  
}
void loop() {
  if (millis() < 2000) {
    digitalWrite(IN1, LOW);                        //Xung IN1 xuống LOW
    digitalWrite(IN2, LOW);                        //Xung IN2 xuống LOW
    //ĐK ĐỘNG CƠ TRÁI
    digitalWrite(IN3, LOW);                        //Xung IN3 xuống LOW
    digitalWrite(IN4, LOW);                        //Xung IN4 xuống LOW
  }

  if (bluetooth.available()) {                   //Tín hiệu đầu vào available
    kytu = bluetooth.read();                     //char kytu gán bằng giá trị bluetooth read
    lenh[i] = kytu;                              //Xét trường hợp vừa lùi vừa rẽ
    if (i == 1) {                                //Dùng lệnh if else để chuyển biến i trong char lenh[]
      i = 0;
    } else {
      i = 1;
    }
    Serial.println(kytu);                        //check kytu đã được gửi đến chưa
    if (kytu == 'm') {
      mode = !mode;                              //Mode chuyển từ 0 thành 1 và từ 1 thành 0
    }
START:

    if (mode == 0) {

      Serial.println(mode);
      //MODE XE ĐIỀU KHIỂN TỪ XA
      //----------
      dung;
      if (kytu == 'q') {                         //Đi thẳng
        dithang(Max, Max);                       //Max speed
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'd') {                         //Đi thẳng
        dithang(200, 200);                       //Đi thẳng bằng 1 cái nhấn thì đi chậm chậm thôi
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'u') {                        //Đi lùi
        dilui(Max / 2, Max / 2);                //Tốc độ lùi bằng nửa tốc độ tiến
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'r') {                        //Rẽ phải
        //rephaiv2(150);
        rephai(60, 200);                        //Bánh bên trái sẽ phải quay nhanh hơn bánh bên phải
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'l') {                        //Rẽ trái
        //retraiv2(150);
        retrai(200, 60);                        //Bánh bên phải sẽ phải quay nhanh hơn bánh bên trái
        s = 0;                                  //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if ((lenh[0] == 'q' && lenh[1] == 'r') || (lenh[0] == 'r' && lenh[1] == 'q')) {
        rephai(60, 200);                       //Bánh bên trái sẽ phải quay nhanh hơn bánh bên phải
        s = 1;                                 //Với các ký tự kép, đk đồng thời 2 nút nhấn thì để sang -1(cho đi lùi) hoặc 1(cho đi tiến)
      }
      //----------
      if ((lenh[0] == 'q' && lenh[1] == 'l') || (lenh[0] == 'l' && lenh[1] == 'q')) {
        retrai(200, 50);                       //Bánh bên phải sẽ phải quay nhanh hơn bánh bên trái
        s = 1;                                 //Với các ký tự kép, đk đồng thời 2 nút nhấn thì để sang -1(cho đi lùi) hoặc 1(cho đi tiến)
      }
      //----------
      if (kytu == 'e') {                       //Lùi trái
        luitrai(200, 50);
        //s = 0;                                 //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'w') {                       //Lùi phai
        luiphai(50, 200);
        s = 0;                                 //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }

      //----------
      if (kytu == 's') {                       //Dừng xe
        if (s == 1) {                          //Ở đây dùng trạng thái của biến s là -1,0,1
          dithang(Max, Max);                   //1 là khi nhấn nút đi thẳng và rẽ phải, khi nhả nút rẽ phải ra
        }                                      //thì app sẽ gửi tín hiệu "s" về, trong khi nút nhấn đi thẳng vẫn còn đang nhấn
        if (s == -1) {                         //mà xe đã dừng. Nên mình dùng biến ở đây để setting khi hàm s được gửi trả trở
          dilui(Max / 2, Max / 2);             //lại
        }
        if (s == 0) {
          dung();
        }
        s = 0;
      }
      //----------
      if (kytu == 'i') {                         //Đi quay vòng vòng bên phải
        iStatus = !iStatus;                      //Đảo trạng thái
        if (iStatus == 1) {
          rephai(25, 255);                       //Quay vòng thì tốc độ của động cơ bên trái max, động cơ bên phải quay chậm
        } else {
          dung;
        }
        s = 0;
      }
      //----------
      if (kytu == 'j') {                         //Đi quay vòng vòng bên trái
        jStatus = !jStatus;                      //Đảo trạng thái
        if (jStatus == 1) {
          retrai(255, 25);                       //Tương tự bên trên, tốc độ động cơ bên phải quay vượt trội hơn
        } else {
          dung;
        }
        s = 0;
      }
      //----------
      if ((lenh[0] == 'u' && lenh[1] == 'l') || (lenh[0] == 'l' && lenh[1] == 'u')) {
        luitrai(200, 50);
        s = -1;                                  //-1 cho chế độ đi lùi
      }
      //----------
      if ((lenh[0] == 'u' && lenh[1] == 'r') || (lenh[0] == 'r' && lenh[1] == 'u')) {
        luiphai(50, 200);
        s = -1;                                  //-1 cho chế độ đi lùi
      }

      //----------
      if (kytu == 'b') {                         //Beep.....Beep
        batcoi;
        analogWrite(beep, 1000);
        delay(200);
        analogWrite(beep, 0);
        s = 0;
      }
      //----------
      if (kytu == 'k') {                        //Bật đèn xe
        LightStatus = !LightStatus;             //Chuyển trạng thái của đèn thành on or off
        if (LightStatus == 0) {                 //0 là trạng thái off của led
          digitalWrite (ledr, LOW);             //cả 2 xung đều LOW
          digitalWrite (ledl, LOW);
        } else {                                //1 là trạng thái mở của led
          digitalWrite (ledr, HIGH);            //cả 2 xung đều HIGH
          digitalWrite (ledl, HIGH);
        }
        s = 0;
      }
      //change mode
      if (kytu == 'm') {                       //Thêm vào để cập nhật lại chế độ và thoát ra ngoài vòng while
        mode = !mode;                          //Mode chuyển từ 0 thành 1 và từ 1 thành 0
        goto START;                            //Trở về phần START để bắt đổi sang chế độ điều khiển từ xa
      }

      //PHẦN ĐỌC KHOẢNG CÁCH, ĐỂ ĐÓ, NÀO CẦN THÌ NÂNG CẤP LÊN
      /*
        while (distance() <= 30) {
          dung();
          if (random(2) == 0) {
            rephaiv2(150);
          } else {
            retraiv2(150);
          }
          delay (100);
        }
      */
    }



    
    //==============================================================================================
    if (mode == 1) {

      //Mode turn left, turn right Ver2
      Serial.println(mode);
      //MODE XE ĐIỀU KHIỂN TỪ XA
      //----------
      if (kytu == 'q') {                         //Đi thẳng
        dithang(Max, Max);                       //Max speed
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'd') {                         //Đi thẳng
        dithang(200, 200);                       //Đi thẳng bằng 1 cái nhấn thì đi chậm chậm thôi
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'u') {                        //Đi lùi
        dilui(Max / 2, Max / 2);                //Tốc độ lùi bằng nửa tốc độ tiến
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'r') {                        //Rẽ phải
        //rephaiv2v2(150);
        rephaiv2(100);                        //Bánh bên trái sẽ phải quay nhanh hơn bánh bên phải
        batxinhanphai();
        tatxinhanphai();
        s = 0;                                   //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 'l') {                        //Rẽ trái
        //retraiv2v2(150);
        retraiv2(100);                        //Bánh bên phải sẽ phải quay nhanh hơn bánh bên trái
        batxinhantrai();
        tatxinhantrai();
        s = 0;                                  //Với các ký tự đơn lẻ gửi về thì mặc định để s = 0
      }
      //----------
      if (kytu == 's') {                       //Dừng xe
        if (s == 1) {                          //Ở đây dùng trạng thái của biến s là -1,0,1
          dithang(Max, Max);                   //1 là khi nhấn nút đi thẳng và rẽ phải, khi nhả nút rẽ phải ra
        }                                      //thì app sẽ gửi tín hiệu "s" về, trong khi nút nhấn đi thẳng vẫn còn đang nhấn
        if (s == -1) {                         //mà xe đã dừng. Nên mình dùng biến ở đây để setting khi hàm s được gửi trả trở
          dilui(Max / 2, Max / 2);             //lại
        }
        if (s == 0) {
          dung();
        }
        s = 0;
      }
      //----------
      if (kytu == 'i') {                         //Đi quay vòng vòng bên phải
        iStatus = !iStatus;                      //Đảo trạng thái
        if (iStatus == 1) {
          rephaiv2(100);                       //Quay vòng thì tốc độ của động cơ bên trái max, động cơ bên phải quay chậm
          batxinhanphai;
        } else {
          dung;
        }
        s = 0;
      }
      //----------
      if (kytu == 'j') {                         //Đi quay vòng vòng bên trái
        jStatus = !jStatus;                      //Đảo trạng thái
        if (jStatus == 1) {
          retraiv2(100);                       //Tương tự bên trên, tốc độ động cơ bên phải quay vượt trội hơn
          batxinhantrai;
        } else {
          dung;
        }
        s = 0;
      }
      //----------


      //----------
      if (kytu == 'b') {                         //Beep.....Beep
        batcoi;
        analogWrite(beep, 1000);
        delay(200);
        analogWrite(beep, 0);
        s = 0;
      }
      //----------
      if (kytu == 'k') {                        //Bật đèn xe
        LightStatus = !LightStatus;             //Chuyển trạng thái của đèn thành on or off
        if (LightStatus == 0) {                 //0 là trạng thái off của led
          digitalWrite (ledr, LOW);             //cả 2 xung đều LOW
          digitalWrite (ledl, LOW);
        } else {                                //1 là trạng thái mở của led
          digitalWrite (ledr, HIGH);            //cả 2 xung đều HIGH
          digitalWrite (ledl, HIGH);
        }
        s = 0;
      }
      if (kytu == 'm') {
        mode = !mode;                              //Mode chuyển từ 0 thành 1 và từ 1 thành 0
      }
    }

  }
}

//-----------------------------------------------------------------------







//CÁC PHẦN HÀM NHIỆM VỤ CON PHÍA SAU:

//ĐI THẲNG
void dithang(int r, int l) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN1, r);                          //Xung IN1 lên HIGH
  analogWrite(IN2, Min);                        //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN3, l);                          //Xung IN3 lên HIGH
  analogWrite(IN4, Min);                        //Xung IN4 xuống LOW
  Serial.println("Di thang");
}
//-----------------------------------------------------------------------
//DỪNG XE
void dung() {
  //ĐK ĐỘNG CƠ PHẢI
  digitalWrite(IN1, LOW);                        //Xung IN1 xuống LOW
  digitalWrite(IN2, LOW);                        //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  digitalWrite(IN3, LOW);                        //Xung IN3 xuống LOW
  digitalWrite(IN4, LOW);                        //Xung IN4 xuống LOW
  Serial.println("Dung");
}
//-----------------------------------------------------------------------
//ĐI LÙI
void dilui(int r, int l) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN2, r);                          //Xung IN2 lên r
  analogWrite(IN1, Min);                        //Xung IN1 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN4, l);                          //Xung IN4 lên l
  analogWrite(IN3, Min);                        //Xung IN3 xuống LOW
  Serial.println("Di lui");
}
//-----------------------------------------------------------------------
//RẼ PHẢI
void rephai(int r, int l) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN1, r);                          //Xung IN1 lên r
  analogWrite(IN2, Min);                        //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN3, l);                          //Xung IN3 xuống l
  analogWrite(IN4, Min);                        //Xung IN4 xuống LOW
  Serial.println("Re phai");
}
//-----------------------------------------------------------------------
void retrai(int r, int l) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN1, r);                          //Xung IN1 lên r
  analogWrite(IN2, Min);                        //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN3, l);                          //Xung IN3 lên l
  analogWrite(IN4, Min);                        //Xung IN4 xuống LOW
  Serial.println("Re trai");
}
//-----------------------------------------------------------------------
//CÁC HÀM ĐI LÙI PHẢI VÀ LÙI TRÁI
void luiphai(int r, int l) {
  dilui(r, l);
}
void luitrai(int r, int l) {
  dilui(r, l);
}
//-----------------------------------------------------------------------
//XI  NHAN SANG ĐƯỜNG KKK
void batxinhanphai() {                          //Khai báo các hàm xi nhan
  digitalWrite(ledr, HIGH);
}
void tatxinhanphai() {
  digitalWrite(ledr, LOW);
}
void batxinhantrai() {
  digitalWrite(ledl, HIGH);
}
void tatxinhantrai() {
  digitalWrite(ledl, LOW);
}
//-----------------------------------------------------------------------
//BẬT CÒI BÁO HIỆU
void batcoi() {
  digitalWrite(beep, HIGH);
  delay(200);
  digitalWrite(beep, LOW);
}
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void rephaiv2(int sp) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN1, Min);                          //Xung IN1 lên r
  analogWrite(IN2, sp);                          //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN3, sp);                          //Xung IN3 xuống l
  analogWrite(IN4, Min);                         //Xung IN4 xuống LOW
}
void retraiv2(int sp) {
  //ĐK ĐỘNG CƠ PHẢI
  analogWrite(IN1, sp);                         //Xung IN1 lên r
  analogWrite(IN2, Min);                        //Xung IN2 xuống LOW
  //ĐK ĐỘNG CƠ TRÁI
  analogWrite(IN3, Min);                        //Xung IN3 xuống l
  analogWrite(IN4, sp);                         //Xung IN4 xuống LOW
}

void luitraiv2(int sp) {

}
void luiphaiv2(int sp) {

}
//-----------------------------------------------------------------------
