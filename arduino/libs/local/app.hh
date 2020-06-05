#pragma once
#include "common.hh"
#include "serial.hh"
#include "light.hh"
#include "button.hh"

class App : public Updatable {
public:
  static const duration_t mcsBuzzTime = 2000;

  enum state {
    loading = -2,
    idle = -1,
    mayo = 2,
    ketchup = 3,
  };

private:
  App(void):
    mFirstLoop(true),
    mLastTime(0),
    mState(state::loading),
    mTargetState(state::idle),
    mLastWinner(0),
    mLightYellow(6),
    mLightRed(7),
    mButtons({ Button(8), Button(9), Button(10), Button(11), Button(12) })
  {
    for (std::size_t cIdx = 0; cIdx < 5; cIdx++) {
      mButtons[cIdx].onPressed([cIdx]{
          std::sout << "button" << cIdx << "::pressed" << std::endl;
        });
      mButtons[cIdx].onReleased([cIdx]{
          std::sout << "button" << cIdx << "::released" << std::endl;
        });
    }
  }

public:
  static App& get(void) {
    if (App::msInstance == 0) {
      App::msInstance = new App();
    }
    return *App::msInstance;
  }

public:
  bool isFirstLoop(void) const { return mFirstLoop; }

  void setup(void)
  {
    gSerial.begin(9600);
    ArduinoSTL_Serial.connect(gSerial);

    std::sout << "app::setup" << std::endl;
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), [](void) {
        App::get().onBuzzerPressed(App::state::mayo);
      }, RISING);
    attachInterrupt(digitalPinToInterrupt(3), [](void) {
        App::get().onBuzzerPressed(App::state::ketchup);
      }, RISING);
  }

  void loop(void)
  {
    time_t      lNow   = millis();
    duration_t  lDelta = lNow - mLastTime;
    mLastTime  = lNow;
    mFirstLoop = false;

    update(lNow, lDelta);
    mLightYellow.update(lNow, lDelta);
    mLightRed.update(lNow, lDelta);
    for (std::size_t cIdx = 0; cIdx < 5; cIdx++) {
      mButtons[cIdx].update(lNow, lDelta);
    }
  }

  void update(time_t /* pTime */, duration_t /* pDuration */) override
  {
    if (mState != state::idle) {
      if (mLastTime - mLastWinner > mcsBuzzTime) {
        reset();
      }
    } else if (mTargetState != mState) {
      winner();
    }
  }

  void winner(void) {
    mState = mTargetState;
    switch (mState) {
    case state::mayo:
      std::sout << "buzzer::mayo" << std::endl;
      mLightYellow.activate();
      break;
    case state::ketchup:
      std::sout << "buzzer::ketchup" << std::endl;
      mLightRed.activate();
      break;
    case state::loading:
    case state::idle:
      break;
    }
  }

  void ready(void)
  {
    std::sout << "app::ready" << std::endl;
    mState = state::idle;
    mTargetState = idle;
    flash(25);
  }

  void reset(void) {
    std::sout << "app::reset" << std::endl;
    mState = state::idle;
    mTargetState = state::idle;
    mLightRed.deactivate();
    mLightYellow.deactivate();
    flash(25);
  }

  void flash(duration_t pSpeed = 150) {
    for (size_t cIdx = 0; cIdx < 10; cIdx++) {
      mLightYellow.activate();
      delay(pSpeed);
      mLightYellow.deactivate();
      mLightRed.activate();
      delay(pSpeed);
      mLightRed.deactivate();
    }
  }

  void onBuzzerPressed(state pState) {
    if ((mState == state::idle) && (mTargetState == state::idle)) {
      std::sout << "buzzer::pressed" << std::endl;
      mTargetState = pState;
      mLastWinner = mLastTime;
    }
  }

private:
  bool       mFirstLoop;
  time_t     mLastTime;
  state      mState;
  state      mTargetState;
  time_t     mLastWinner;
  Light      mLightYellow;
  Light      mLightRed;
  Button     mButtons[5];

private:
  static App* msInstance;
};

App* App::msInstance = 0;
