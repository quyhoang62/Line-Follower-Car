byte SpMax = 190;
int demcross = 0;
byte previousStatus = 0;
byte recentStatus = 0;

// Hàm thiết lập
void setup() {
  Serial.begin(9600);
  
  // Thiết lập các chân điều khiển động cơ
  for (int i = 6; i <= 9; i++) pinMode(i, OUTPUT);
  
  // Thiết lập các chân cảm biến
  for (int i = A1; i <= A5; i++) pinMode(i, INPUT);
}

void loop() {
  doline();
}

// Hàm điều khiển hướng di chuyển của robot
void doline() {
  previousStatus = recentStatus;
  recentStatus = trangThai5CamBien();
  Serial.println(demcross);

  int status = trangThai5CamBien();

  // Kiểm tra trạng thái để điều khiển
  if (status == 100) {
    if (previousStatus != 100) demcross++;
    Rephai(200, 200);
    delay(300);
  } else if (status == 200) {
    // Dừng lại hoặc không làm gì khi trạng thái là 200
  } else {
    dieuKhienRobot(status);
  }
}

// Hàm điều khiển tốc độ và hướng robot dựa trên trạng thái cảm biến
void dieuKhienRobot(int status) {
  int speedL = SpMax, speedR = SpMax;
  
  switch (status) {
    case 0: break;
    case 1: speedL -= 50; speedR -= 5; break;
    case 2: speedL -= 100; speedR += 30; break;
    case 3: speedL = 0; speedR += 60; break;
    case -1: speedL -= 5; speedR -= 50; break;
    case -2: speedL += 30; speedR -= 100; break;
    case -3: speedL += 60; speedR = 0; break;
  }
  
  Tien(speedL, speedR);
}

// Kiểm tra trạng thái cảm biến dựa trên mức độ lệch
int trangThai5CamBien() {
  int sens1 = IFSensor(A1);
  int sens2 = IFSensor(A2);
  int sens3 = IFSensor(A3);
  int sens4 = IFSensor(A4);
  int sens5 = IFSensor(A5);

  int sensorSum = abs(sens1) + abs(sens2) + abs(sens3) + abs(sens4) + abs(sens5);

  if (sensorSum == 0) return 200;
  if (sensorSum == 5) return 100;
  
  return lechvach5mat(sens1, sens2, sens3, sens4, sens5);
}

// Tính toán mức độ lệch của robot dựa trên cảm biến
int lechvach5mat(int sens1, int sens2, int sens3, int sens4, int sens5) {
  int lechvach = -2 * sens1 - sens2 + sens4 + 2 * sens5;
  
  if (lechvach == -3) return -2;
  if (lechvach == -2) return -3;
  if (lechvach == -1) return -1;
  if (lechvach == 0) return 0;
  if (lechvach == 1) return 1;
  if (lechvach == 2) return 3;
  if (lechvach == 3) return 2;

  return 0;
}

// Hàm kiểm tra cảm biến
boolean IFSensor(byte PinNumb) {
  return !digitalRead(PinNumb);
}

// Hàm điều khiển robot tiến, trái, phải, lui
void dieuKhienDongCo(byte speedL, byte speedR, byte in1A, byte in2A, byte in1B, byte in2B) {
  analogWrite(11, speedL);
  analogWrite(10, speedR);
  
  digitalWrite(6, in1A);
  digitalWrite(7, in2A);
  digitalWrite(8, in1B);
  digitalWrite(9, in2B);
}

void Tien(byte speedL, byte speedR) {
  dieuKhienDongCo(speedL, speedR, 1, 0, 1, 0);
}

void Lui(byte speedL, byte speedR) {
  dieuKhienDongCo(speedL, speedR, 0, 1, 0, 1);
}

void Rephai(byte speedL, byte speedR) {
  dieuKhienDongCo(speedL, speedR, 1, 0, 0, 1);
}

void Retrai(byte speedL, byte speedR) {
  dieuKhienDongCo(speedL, speedR, 0, 1, 1, 0);
}
