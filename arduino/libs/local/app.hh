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
    mLastTime(0),
    mState(state::loading),
    mTargetState(state::idle),
    mLastWinner(0),
    mLightGreen(6),
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
  void setup(void)
  {
    initialize();
    negociate();
    ready();
  }

  void initialize(void)
  {
    gSerial.begin(9600);
    ArduinoSTL_Serial.connect(gSerial);
    std::sout << "app::initialize" << std::endl;

    mLightGreen.activate();
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), [](void) {
        App::get().onBuzzerPressed(App::state::mayo);
      }, RISING);
    attachInterrupt(digitalPinToInterrupt(3), [](void) {
        App::get().onBuzzerPressed(App::state::ketchup);
      }, RISING);
  }

  void negociate(void)
  {
    std::sout << "app::negociating" << std::endl;
    char l_buf[16] = { 0x0 };
    std::sin.read(l_buf, 15);
    std::sout << "got [" << std::string(l_buf, 15) << "]" << std::endl;
  }

  void ready(void)
  {
    std::sout << "app::ready" << std::endl;
    mLightRed.activate();
    mState = state::idle;
    mTargetState = idle;
  }

  void loop(void)
  {
    time_t      lNow   = millis();
    duration_t  lDelta = lNow - mLastTime;
    mLastTime  = lNow;

    update(lNow, lDelta);
    mLightGreen.update(lNow, lDelta);
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
      mLightGreen.activate();
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

  void reset(void) {
    std::sout << "app::reset" << std::endl;
    mState = state::idle;
    mTargetState = state::idle;
    flash(25);
  }

  void flash(duration_t pSpeed = 150) {
    for (size_t cIdx = 0; cIdx < 10; cIdx++) {
      mLightGreen.deactivate();
      mLightRed.deactivate();
      delay(pSpeed);
      mLightGreen.activate();
      mLightRed.activate();
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
  time_t     mLastTime;
  state      mState;
  state      mTargetState;
  time_t     mLastWinner;
  Light      mLightGreen;
  Light      mLightRed;
  Button     mButtons[5];

private:
  static App* msInstance;
};

App* App::msInstance = 0;
