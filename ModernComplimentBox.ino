#include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <string.h>
#include <hd44780.h>						// main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
// declare the lcd object for auto i2c address location
hd44780_I2Cexp lcd;

const int COMPLIMENT_BUTTON_PIN = 7;
const int QUOTE_BUTTON_PIN = 6;

const int numCompliments = 26;
const int numQuotes = 46;
int index = 0;

const char quotes[] PROGMEM = {"The purpose of our lives is to be happy. DL~No one can make you feel inferior without your consent. ER~Difficulties in life are intended to make us better, not bitter. DR~Your attitude, not your aptitude, will determine your altitude. ZZ~Do not wait to strike till the iron is hot, but make it hot by striking. WBY~Life is what happens when you're busy making other plans. AS~You are never too old to set another goal or to dream a new dream. CSL~Don't watch the clock; do what it does. Keep going. SL~The mind is everything. What you think you become. B~Success is not in what you have, but who you are. BB~The best preparation for tomorrow is doing your best today. HJB~If you want to achieve greatness stop asking for permission. A~It does not matter how slowly you go as long as you do not stop. C~The only way to do great work is to love what you do. SJ~The purpose of our lives is to be happy. DL~Do not wait for leaders; do it alone, person to person. MT~If you want to achieve greatness stop asking for permission. A~You are never too old to set another goal or to dream a new dream. CSL~If you want to achieve greatness stop asking for permission. A~It does not matter how slowly you go as long as you do not stop. C~Success is not in what you have, but who you are. BB~The only way to do great work is to love what you do. SJ~The best preparation for tomorrow is doing your best today. HJB~The mind is everything. What you think you become. B~You must be the change you wish to see in the world. MG~Believe you can and you're halfway there. TR~The only way to do great work is to love what you do. SJ~Your time is limited, don't waste it living someone else's life. SJ~In the middle of difficulty lies opportunity. AE~The future belongs to those who believe in the beauty of their dreams. ER~The best way to predict the future is to create it. PD~Happiness is not by chance, but by choice. JR~The purpose of our lives is to be happy. DL~No one can make you feel inferior without your consent. ER~Difficulties in life are intended to make us better, not bitter. DR~Your attitude, not your aptitude, will determine your altitude. ZZ~The only person you are destined to become is the person you decide to be. RW~You must be the change you wish to see in the world. MG~Believe you can and you're halfway there. TR~The only way to do great work is to love what you do. SJ~Your time is limited, don't waste it living someone else's life. SJ~In the middle of difficulty lies opportunity. AE~The future belongs to those who believe in the beauty of their dreams. ER~Success is not final, failure is not fatal: It is the courage to continue that counts. WC~The best way to predict the future is to create it. PD~Happiness is not by chance, but by choice. JR"};
const char compliments[] PROGMEM = {"You are a wonderful human.~You have a great sense of humor.~Your smile is contagious.~You're a fantastic listener.~Your kindness is inspiring.~You have a beautiful soul.~I appreciate your creativity.~You always bring positivity into the room.~Your perseverance is admirable.~You're a true team player.~Your energy is uplifting.~Your intelligence shines through in everything you do.~You have a wonderful way with words.~I love your enthusiasm for life.~You're incredibly thoughtful.~Your work ethic is commendable.~You have a knack for making people feel comfortable.~Your generosity knows no bounds.~You're a constant source of inspiration.~Your presence makes a difference.~You have a great eye for detail.~Your passion is contagious.~You're a natural leader.~Your commitment is unwavering.~I admire your honesty.~You radiate confidence."};

const int LCD_WIDTH = 20;
const int MAX_LINES = 4;
void setup() {
  int status = lcd.begin(LCD_WIDTH, MAX_LINES);
  if(status) // non zero status means it was unsuccesful
	{
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

  pinMode(QUOTE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(COMPLIMENT_BUTTON_PIN, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print(F("  Noah, I hope you  "));
  lcd.setCursor(0, 1);
  lcd.print(F("   are doing well.  "));
  lcd.setCursor(0, 2);
  lcd.print(F("Push For Compliments"));
  lcd.setCursor(0, 3);
  lcd.print(F(" Or For Inspiration "));
}

void loop() {
  int old_index = index;
  randomSeed(analogRead(5));
  if (!digitalRead(COMPLIMENT_BUTTON_PIN)) {
    // Get new random number in bounds
    while (old_index == index) {
      index = random(0, numCompliments);
    }
    // Print
    lcd.clear();
    lcd.print("Index is: " + String(index));
    delay(500);
    lcd.clear();
    char* tmp = (char*) calloc(strlen(compliments) + 1, sizeof(char));
    char* result = parseData(tmp, index);
    // TODO: Print the string
    free(result);
    free(tmp);
  } else if (!digitalRead(QUOTE_BUTTON_PIN)) {
    // Get new random number in bounds
    while (old_index == index) {
      index = random(0, numQuotes);
    }
    // Print
    char* tmp = (char*) calloc(strlen(quotes) + 1, sizeof(char));
    strcpy(tmp, quotes);
    char* result = parseData(tmp, index);
    printString(result);
    free(result);
    free(tmp);
  }
  delay(500); // TODO: Is it necessary to delay at all?
}

void clearLCD() {
  lcd.clear();
}

char* parseData(char* data, int num) {
  // TODO: data is NULL when it gets here
  int i = 0;
  lcd.clear();
  lcd.print(data);
  lcd.setCursor(0,0);
  lcd.print("#");
  delay(500);
  char* tok = strtok(data, "~");
  while (tok != NULL) {
    lcd.clear();
    lcd.print(tok);
    delay(300);
    if (i == num) {
      lcd.clear();
      lcd.print(tok);
      delay(5000);
      return tok;
    }
    i++;
    tok = strtok(NULL, "~");
  }
  lcd.clear();
  lcd.print("Returned NULL");
  delay(5000);
  
  return NULL;
}

void printString(char* s) {
  lcd.print(s);
}
