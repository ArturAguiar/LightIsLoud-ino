struct Button
{
  int dataPin;
  boolean pressed = false;  
};

Button backButton;
Button nextButton;

int currentSetting = 0;

void setup() 
{
  backButton.dataPin = 2;
  nextButton.dataPin = 3;
  
  pinMode(backButton.dataPin, INPUT);
  pinMode(nextButton.dataPin, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  boolean backButtonWasPressed = digitalRead(backButton.dataPin) == LOW;
  boolean nextButtonWasPressed = digitalRead(nextButton.dataPin) == LOW;
  
  //Serial.print("Back: ");
  //Serial.println(backButtonWasPressed);
  
  if (!backButton.pressed && backButtonWasPressed)
  {
    currentSetting--;
    Serial.println("Back button pressed!");
    Serial.println(currentSetting);
  }
  else if (!nextButton.pressed && nextButtonWasPressed)
  {
    currentSetting++;
    Serial.println("Next button pressed!");
    Serial.println(currentSetting);
  }
  
  backButton.pressed = backButtonWasPressed;
  nextButton.pressed = nextButtonWasPressed;
}
