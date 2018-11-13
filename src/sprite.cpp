class Sprite {

public:

  Sprite();
private:
  
};

Sprite::Sprite() {

  vector<vec3> vertices = {{-1.0f, 1.0f, 0.0f},
                           {-1.0f,-1.0f, 0.0f},
                           { 1.0f,-1.0f, 0.0f},

                           { 1.0f,-1.0f, 0.0f},
                           { 1.0f, 1.0f, 0.0f},
                           {-1.0f, 1.0f, 0.0f}};

  vector<vec2> texCoords = {{0.0f, 1.0f},
                            {0.0f, 0.0f},
                            {1.0f, 0.0f},

                            {1.0f, 0.0f},
                            {1.0f, 1.0f},
                            {0.0f, 1.0f}};
}
