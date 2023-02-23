#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <raylib.h>

struct Parameters {
  float posX;
  float posY;
  float width;
  float height;
};

struct Controls {
  uint32_t buttonUp;
  uint32_t buttonDown;
};

struct Circle {
  float x;
  float y;
  float radius;
};

class Object {
  public:
    Object() {};
       
    Vector2 pos = {0.0,0.0};
    Vector2 velocity = {0.0,0.0};
    
  protected:
    virtual void SyncPos() {};
    
    virtual void Draw() {};
};

class Paddle : public Object {
  public:
    Paddle() {};
    Paddle(Parameters param, Controls controls) {
      this->obj = {param.posX, param.posY, param.width, param.height};
      this->controls = controls;
      SyncPos();
    }
    
    void MoveUp() {this->obj.y -= velocity.y*GetFrameTime();}
    void MoveDown() {this->obj.y += velocity.y*GetFrameTime();}
    
    void Input() {
      if(IsKeyDown(controls.buttonUp) && obj.y >= 0.0f) 
      {
        MoveUp();
      }
      else if(IsKeyDown(controls.buttonDown) && obj.y <= GetScreenHeight()-obj.height) 
      {
        MoveDown();
      }
    }
    
    void Draw() {
      SyncPos();
      DrawRectangleRec(obj, RAYWHITE);
    }
    
    Rectangle obj;
    Controls controls;
    int score;
    bool DidScore = false;
    
  private:
    void SyncPos() {this->pos = {obj.x, obj.y};}
};

class Ball : public Object {
  public:
    Ball() {};
    Ball(float x, float y, float radius) {
      this->obj = {x, y, radius};
      SyncPos();
    }
    
    void SetRandDirection() {
      switch(rand()%4) {
        case 0:
          this->velocity.x*=-1;
          break;
        case 2:
          this->velocity.y*=-1;
          break;
        default:
          break;
      }
    }
    
    void Draw() {
      VelocityGain();
      UpdatePosition();
      SyncPos();
      DrawCircle(obj.x, obj.y, obj.radius, RAYWHITE);
    }
    
    void Reset() {
      this->obj.x = GetScreenWidth()/2.0f;
      this->obj.y = GetScreenHeight()/2.0f;
      this->velocity = {0.0f,0.0f};
    }
    
    Circle obj;
    float timeSinceGain;
    bool DidGain = false;
    
  private:
    void VelocityGain() {
      if(GetTime()-timeSinceGain >= 3.0f) {
        DidGain = true;
        this->velocity.x += 20.0f;
        this->velocity.y += 20.0f;
        timeSinceGain = GetTime();
      }
    }
    
    void UpdatePosition() {
      this->obj.x += this->velocity.x*GetFrameTime();
      this->obj.y += this->velocity.y*GetFrameTime();
    }
    
    void SyncPos() {this->pos = {obj.x, obj.y};}
};

#endif
