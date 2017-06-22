

void handleIrInput()
{
  InputCommand command = readCommand(defaultHoldDelay);

  if (command != InputCommand::None) {
    Serial.print("command: ");
    Serial.println((int) command);
  }

  switch (command) {
    case InputCommand::Up: {
        adjustPattern(true);
        break;
      }
    case InputCommand::Down: {
        adjustPattern(false);
        break;
      }
    case InputCommand::Power: {
        setPower(power == 0 ? 1 : 0);
        break;
      }
    case InputCommand::BrightnessUp: {
        adjustBrightness(true);
        break;
      }
    case InputCommand::BrightnessDown: {
        adjustBrightness(false);
        break;
      }
    case InputCommand::PlayMode: { // toggle pause/play
        autoplayEnabled = !autoplayEnabled;
        break;
      }

    // pattern buttons

    case InputCommand::Pattern1: {
        setPattern(0);
        break;
      }
    case InputCommand::Pattern2: {
        setPattern(1);
        break;
      }
    case InputCommand::Pattern3: {
        setPattern(2);
        break;
      }
    case InputCommand::Pattern4: {
        setPattern(3);
        break;
      }
    case InputCommand::Pattern5: {
        setPattern(4);
        break;
      }
    case InputCommand::Pattern6: {
        setPattern(5);
        break;
      }
    case InputCommand::Pattern7: {
        setPattern(6);
        break;
      }
    case InputCommand::Pattern8: {
        setPattern(7);
        break;
      }
    case InputCommand::Pattern9: {
        setPattern(8);
        break;
      }
    case InputCommand::Pattern10: {
        setPattern(9);
        break;
      }
    case InputCommand::Pattern11: {
        setPattern(10);
        break;
      }
    case InputCommand::Pattern12: {
        setPattern(11);
        break;
      }

    // custom color adjustment buttons

    case InputCommand::RedUp: {
        solidColor.red += 8;
        setSolidColor(solidColor);
        break;
      }
    case InputCommand::RedDown: {
        solidColor.red -= 8;
        setSolidColor(solidColor);
        break;
      }
    case InputCommand::GreenUp: {
        solidColor.green += 8;
        setSolidColor(solidColor);
        break;
      }
    case InputCommand::GreenDown: {
        solidColor.green -= 8;
        setSolidColor(solidColor);
        break;
      }
    case InputCommand::BlueUp: {
        solidColor.blue += 8;
        setSolidColor(solidColor);
        break;
      }
    case InputCommand::BlueDown: {
        solidColor.blue -= 8;
        setSolidColor(solidColor);
        break;
      }

    // color buttons

    case InputCommand::Red: {
        setSolidColor(CRGB::Red);
        break;
      }
    case InputCommand::RedOrange: {
        setSolidColor(CRGB::OrangeRed);
        break;
      }
    case InputCommand::Orange: {
        setSolidColor(CRGB::Orange);
        break;
      }
    case InputCommand::YellowOrange: {
        setSolidColor(CRGB::Goldenrod);
        break;
      }
    case InputCommand::Yellow: {
        setSolidColor(CRGB::Yellow);
        break;
      }

    case InputCommand::Green: {
        setSolidColor(CRGB::Green);
        break;
      }
    case InputCommand::Lime: {
        setSolidColor(CRGB::Lime);
        break;
      }
    case InputCommand::Aqua: {
        setSolidColor(CRGB::Aqua);
        break;
      }
    case InputCommand::Teal: {
        setSolidColor(CRGB::Teal);
        break;
      }
    case InputCommand::Navy: {
        setSolidColor(CRGB::Navy);
        break;
      }

    case InputCommand::Blue: {
        setSolidColor(CRGB::Blue);
        break;
      }
    case InputCommand::RoyalBlue: {
        setSolidColor(CRGB::RoyalBlue);
        break;
      }
    case InputCommand::Purple: {
        setSolidColor(CRGB::Purple);
        break;
      }
    case InputCommand::Indigo: {
        setSolidColor(CRGB::Indigo);
        break;
      }
    case InputCommand::Magenta: {
        setSolidColor(CRGB::Magenta);
        break;
      }

    case InputCommand::White: {
        setSolidColor(CRGB::White);
        break;
      }
    case InputCommand::Pink: {
        setSolidColor(CRGB::Pink);
        break;
      }
    case InputCommand::LightPink: {
        setSolidColor(CRGB::LightPink);
        break;
      }
    case InputCommand::BabyBlue: {
        setSolidColor(CRGB::CornflowerBlue);
        break;
      }
    case InputCommand::LightBlue: {
        setSolidColor(CRGB::LightBlue);
        break;
      }
  }
}

