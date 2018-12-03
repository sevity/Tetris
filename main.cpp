#include <SFML/Graphics.hpp>
using namespace std;


//int offset[7] = {}
int blocks[7][4][4] = {
    {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,0,0,0},
    },
    {
    {0,1,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {0,0,0,0},
    },
    {
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    {1,0,0,0},
    },
    {
    {1,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0},
    },
    {
    {1,0,0,0},
    {1,1,0,0},
    {1,0,0,0},
    {0,0,0,0},
    },
    {
    {1,0,0,0},
    {1,0,0,0},
    {1,1,0,0},
    {0,0,0,0},
    },
    {
    {0,1,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    },
};
float block_size = 20;
const int w_cnt = 10;
const int h_cnt = 20;
int data[h_cnt][w_cnt] = {0};

int main()
{
    sf::Color color_map[] = {sf::Color(100,100,100,255), sf::Color::Green, sf::Color::Blue, sf::Color::Red, sf::Color::Yellow, sf::Color::White, sf::Color::Magenta, sf::Color::Cyan};
    sf::RenderWindow window(sf::VideoMode(w_cnt * block_size, h_cnt * block_size), "Tetris");
    sf::RectangleShape shape(sf::Vector2f(block_size, block_size));
    shape.setFillColor(sf::Color::Green);

    bool rotate = false;
    int dx=0;
    int kind = 1;

    sf::Clock clock;

    int cx=4, cy=0;
    auto check = [&]()
    {
        for(int i=0;i<4;i++)for(int j=0;j<4;j++)
        {
            if(blocks[kind][i][j]==0) continue;
            if(j+cx<0||j+cx>=w_cnt||i+cy>=h_cnt) return false;
            if(data[cy+i][cx+j]) return false;
        }
        return true;
    };

    int center_x[7] = {0};
    int center_y[7] = {0};
    for(int k=0;k<7;k++)for(int y=0;y<4;y++)for(int x=0;x<4;x++)if(blocks[k][y][x]==2) {center_x[k]=x;center_y[k]=y;}
    while (window.isOpen())
    {
        auto delete_line = [&]()
        {
            vector<vector<int>> vv;
            for(int y=0;y<h_cnt;y++)
            {
                vector<int> v;
                int cnt = 0;
                for(int x=0;x<w_cnt;x++)
                {
                    v.push_back(data[y][x]);
                    if(data[y][x]) cnt++;
                }
                if(cnt<w_cnt) vv.push_back(v);
            }
            for(int y=0;y<h_cnt;y++)for(int x=0;x<w_cnt;x++)data[y][x]=0;
            for(int y=h_cnt-1;y>=0;y--)
            {
                if(vv.empty()) break;
                vector<int> v = vv.back();
                vv.pop_back();
                for(int x=0;x<w_cnt;x++)
                    data[y][x]=v[x];
            }
        };
        auto go_down = [&]()
        {
            cy++;
            if(check()==false)
            {
                cy--;
                for(int i=0;i<4;i++)for(int j=0;j<4;j++)if(blocks[kind][i][j])data[cy+i][cx+j]=kind+1;
                delete_line();
                cx=4,cy=0;
                kind = rand()%7;
                return true;
            }
            return false;
        };
        static float prev = clock.getElapsedTime().asSeconds();
        if(clock.getElapsedTime().asSeconds() - prev >= 0.5)
        {
            prev = clock.getElapsedTime().asSeconds();
            go_down();

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Up) rotate = true;
                else if(event.key.code == sf::Keyboard::Right)
                {
                    cx++;
                    if(check()==false) cx--;
                }
                else if(event.key.code == sf::Keyboard::Left)
                {
                    cx--;
                    if(check()==false) cx++;
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    go_down();
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    while(!go_down());
                }

            }
            
        }
        auto Rotate = [&]()
        {
            int len = 0;
            for(int i=0;i<4;i++)for(int j=0;j<4;j++)
                if(blocks[kind][i][j]) len = max(max(i,j)+1,len);
            printf("len:%d\n", len);

            int d[4][4]={0};
            for(int y=0;y<len;y++)for(int x=0;x<len;x++)
                if(blocks[kind][y][x]) d[len-1-x][y]=1;
            for(int y=0;y<4;y++)for(int x=0;x<4;x++)
                blocks[kind][y][x]=d[y][x];
        };

        if (rotate)
        {
            Rotate();
            if(!check()) {Rotate(), Rotate(), Rotate();};
            rotate=false;
        }



        window.clear(color_map[0]);
        auto draw_map = [&]()
        {
            for(int y=0;y<h_cnt;y++)for(int x=0;x<w_cnt;x++)
            {
                shape.setFillColor(color_map[data[y][x]]);
                shape.setPosition(sf::Vector2f((x)*block_size, (y)*block_size));
                window.draw(shape);
            }

        };
        auto draw_shape = [&]()
        {
            shape.setFillColor(color_map[kind+1]);
            for(int i=0;i<4;i++)
            {
                for(int j=0;j<4;j++)
                {
                    if(blocks[kind][i][j])
                    {
                        shape.setPosition(sf::Vector2f((cx+j)*block_size, (cy+i)*block_size));
                        window.draw(shape);
                    }
                }
            }
        };

        draw_map();
        draw_shape();
        window.display();
    }

    return 0;
}
