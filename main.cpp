//1. Create Game Window
//2. Draw a cell
//3. Draw blocks
//4. move left and right and down
//5. draw world, timer down, collision detection, next block
//6. clear lines


#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int block[7][4][4] = 
{
    1,0,0,0,
    1,0,0,0,
    1,0,0,0,
    1,0,0,0,

    1,0,0,0,
    1,1,0,0,
    0,1,0,0,
    0,0,0,0,

    0,1,0,0,
    1,1,0,0,
    1,0,0,0,
    0,0,0,0,

    1,0,0,0,
    1,1,0,0,
    1,0,0,0,
    0,0,0,0,

    1,0,0,0,
    1,0,0,0,
    1,1,0,0,
    0,0,0,0,

    0,1,0,0,
    0,1,0,0,
    1,1,0,0,
    0,0,0,0,

    1,1,0,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,

};

const int cell_size = 20;
const int w_cnt = 10;
const int h_cnt = 20;
const Color color_map[] = {
    Color::Green, Color::Blue, Color::Red, Color::Yellow,
    Color::White, Color::Magenta, Color::Cyan};
int world[h_cnt][w_cnt] = {0};

int main(void)
{
    RenderWindow window(VideoMode(w_cnt * cell_size, h_cnt * cell_size), "Tetris");
    RectangleShape cell(Vector2f(cell_size, cell_size));

    int kind, cx, cy;

    auto new_block = [&]()
    {
        kind = rand()%7;
        cx = w_cnt/2;
        cy = 0;
    };
    new_block();
    
    auto check_block = [&]()
    {
        for(int y=0;y<4;y++)for(int x=0;x<4;x++)
        {
            if(block[kind][y][x]==0) continue; 
            if(x+cx<0||x+cx>=w_cnt||y+cy>=h_cnt) return false; // hit wall
            if(world[cy+y][cx+x]) return false; // collision with world
        }
        return true;
    };
    auto rotate = [&]()
    {
        int len = 0; // check rotate size
        for(int y = 0;y<4;y++)for(int x=0;x<4;x++)
            if(block[kind][y][x]) len=max(max(x,y)+1,len);

        int d[4][4] = {0};
        //rotate counter clock wise 90 degree
        for(int y=0;y<len;y++)for(int x=0;x<len;x++)
            if(block[kind][y][x]) d[len-1-x][y] = 1;
        for(int y=0;y<4;y++)for(int x=0;x<4;x++)
            block[kind][y][x] = d[y][x];
    };
    auto clear_lines = [&]()
    {
        int to = h_cnt - 1;
        //from bottom line to top line..
        for(int from = h_cnt-1;from>=0;from--)
        {
            int cnt = 0;
            for(int x=0;x<w_cnt;x++)if(world[from][x]) cnt++;
            // if current line is not full, copy it (survived line)
            if(cnt<w_cnt)
            {
                for(int x=0;x<w_cnt;x++)world[to][x]=world[from][x];
                to--;
            }
            //otherwise it will be deleted (clear the line)
        }
    };
    auto go_down = [&]()
    {
        cy++;
        if(check_block() == false) // hit bottom
        {
            cy--;
            for(int y=0;y<4;y++)for(int x=0;x<4;x++)
                if(block[kind][y][x])
                {
                    world[cy+y][cx+x] = kind + 1;//+1 for avoiding 0
                }
            clear_lines();
            //start next block
            new_block();
            return false;
        }
        return true;
    };


    Clock clock;
    while(window.isOpen())
    {
        static float prev = clock.getElapsedTime().asSeconds();
        if(clock.getElapsedTime().asSeconds() - prev >= 0.3)
        {
            prev = clock.getElapsedTime().asSeconds();
            go_down();
        }
        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed) window.close();
            if(e.type == Event::KeyPressed)
            {
                if(e.key.code == Keyboard::Left)
                {
                    cx--;
                    if(check_block()==false) cx++;
                }
                if(e.key.code == Keyboard::Right)
                {
                    cx++;
                    if(check_block()==false) cx--;
                }
                if(e.key.code == Keyboard::Up)
                {
                    rotate();
                    //since roate mean 90 degree rotation call 3 times more to cancel it
                    if(check_block()==false) {rotate();rotate();rotate();}
                }
                if(e.key.code == Keyboard::Down)
                {
                    go_down();
                }
                if(e.key.code == Keyboard::Space)
                {
                    while(go_down()==true);
                }
            }
        }
        window.clear(Color(0xaaaaaaff));

        auto draw_world = [&]()
        {
            for(int y=0;y<h_cnt;y++)for(int x=0;x<w_cnt;x++)
                if(world[y][x])
                {
                    cell.setFillColor(color_map[world[y][x] - 1]);
                    cell.setPosition(Vector2f(x*cell_size, y*cell_size));
                    window.draw(cell);
                }

        };
        draw_world();


        // define lambda function
        auto draw_block = [&]()
        {
            cell.setFillColor(color_map[kind]);
            for(int y=0;y<4;y++)for(int x=0;x<4;x++)
                if(block[kind][y][x])
                {
                    cell.setPosition(Vector2f((cx+x)*cell_size, (cy+y)*cell_size));
                    window.draw(cell);
                }
        };

        // call the lambda function
        draw_block();


        window.display();
    }

    return 0;
}
