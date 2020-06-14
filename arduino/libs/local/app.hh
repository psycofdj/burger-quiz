#pragma once
#include "common.hh"
#include "serial.hh"
#include "light.hh"
#include "button.hh"
#include "buzzer.hh"

class App : public Updatable {
public:
  static const duration_t mcsBuzzTime = 0;

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
    mBuzzerMayo(id::d2),
    mBuzzerKetchup(id::d3),
    mLightRed(id::d6),
    mLightBlue(id::d7),
    mButtons({ Button(id::d8), Button(id::d9), Button(id::d10), Button(id::d11), Button(id::d12), Button(id::d13) })
  {
    for (std::size_t cIdx = 0; cIdx < 6; cIdx++) {
      mButtons[cIdx].onPressed([cIdx]{
          std::sout << "button" << cIdx << "::pressed" << std::endl;
        });
      mButtons[cIdx].onReleased([cIdx]{
          std::sout << "button" << cIdx << "::released" << std::endl;
        });
    }
    mBuzzerMayo.onTriggered([this](void) {
        this->onBuzzerPressed(App::state::mayo);
      });
    mBuzzerKetchup.onTriggered([this](void) {
        this->onBuzzerPressed(App::state::ketchup);
      });
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
    mLightRed.on();
  }

  void negociate(void)
  {
    std::sout << "app::negociating" << std::endl;
    char l_buf[16] = { 0x0 };
    std::sin.read(l_buf, 15);
    std::string l_value(l_buf, 13);
    if (l_value != "client::ready") {
      std::sout << "unexpected client message: [" << l_value << "]" << std::endl;
      exit(1);
    }
  }

  void ready(void)
  {
    std::sout << "app::ready" << std::endl;
    mLightBlue.on();
    mState = state::idle;
    mTargetState = idle;
    mLastTime = millis();
  }

  void loop(void)
  {
    time_t      lNow   = millis();
    duration_t  lDelta = lNow - mLastTime;
    mLastTime  = lNow;

    update(lNow, lDelta);
    mLightBlue.update(lNow, lDelta);
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
      break;
    case state::ketchup:
      std::sout << "buzzer::ketchup" << std::endl;
      break;
    case state::loading:
    case state::idle:
      break;
    }
  }

  void reset(void) {
    std::sout << "app::reset" << std::endl;
    flash(25);
    mState = state::idle;
    mTargetState = state::idle;
  }

  void flash(duration_t pSpeed = 150) {
    for (size_t cIdx = 0; cIdx < 10; cIdx++) {
      mLightBlue.off();
      mLightRed.off();
      delay(pSpeed);
      mLightBlue.on();
      mLightRed.on();
      delay(pSpeed);
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
  Buzzer     mBuzzerMayo;
  Buzzer     mBuzzerKetchup;
  Light      mLightRed;
  Light      mLightBlue;
  Button     mButtons[6];

private:
  static App* msInstance;
};

App* App::msInstance = 0;
