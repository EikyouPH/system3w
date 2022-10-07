const int pinLux = A0;

//Round a float to 2 decimals
float round2(float var)
{
  float value = (int)(var * 100 + .5);
  return (float)value / 100;
}

float mesureCapteurs(pinLux)
{
    analogRead(pinLux);
    float lux = round2(analogRead(pinLux));
    return lux;
    
}