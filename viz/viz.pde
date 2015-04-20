import processing.serial.*;

Serial myPort;  // The serial port
int MINBAND = 1;
int LOW = 20;
int MID = 40;
int HIGH = 60;

void setup() {
  size(displayWidth, displayHeight);
  // List all the available serial ports
  println(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[0], 9600);
  background(0);
}

void draw() {
   while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil('\n');   
    if (inBuffer != null) {
      String[] values = inBuffer.trim().split(":");
      int i = 0;
      int v = 0;
      try {
        i= Integer.parseInt(values[0]);
        v = Integer.parseInt(values[1]) * 20;
      }
      catch(Exception e){
        continue;
      }
      int xpos = (int)((i-MINBAND)*(displayWidth)/((float)(HIGH-MINBAND)));
      fill(0);
      rect (xpos, 0,  10, height);
      if (i < LOW){
        fill(255, 0, 0);
      }
      else if (i < MID){
        fill(0, 255, 0);
      }
      else if (i < HIGH){
        fill(0, 0, 255);
      }
      rect (xpos, height-v,  10, v);
    }
  }
}

boolean sketchFullScreen() {
  return true;
}
