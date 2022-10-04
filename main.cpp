#include <simplecpp>
#include "shooter.h"
#include "bubble.h"
#include <sstream>
#include "healthbar.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;
/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

void remove_health(vector <Health> &health,int i){
    //to remove the health on each hit with the shooter and bubble
    health.erase(health.begin()+i);
}

void collision(vector<Bubble> &bubbles, vector<Bullet> &bullets, int &curr_score){
    //to check for collisions between the bubbles and the bullets
    double distance ;
    for(unsigned int i=0;i<bubbles.size();i++){
        for (unsigned int j=0;j<bullets.size();j++){
            distance = pow((pow(bubbles[i].get_center_x()-bullets[j].get_center_x(),2)+ pow(bubbles[i].get_center_y()-bullets[j].get_center_y(),2)),0.5);
            
            //checking the condition of bullet and bubble, the 3 in the denominator was taked experimeantally to account for the size of the bullet
            if (distance<bubbles[i].get_radius()+bullets[j].get_height()/3){
                int x=bubbles[i].get_center_x(), y=bubbles[i].get_center_y(), current_radius=bubbles[i].get_radius(),current_vy=bubbles[i].get_vy();
                bubbles.erase(bubbles.begin()+i);
                bullets.erase(bullets.begin()+j);
                
                //creating the new bubble of half size here and updating the score
                if (current_radius==4*BUBBLE_DEFAULT_RADIUS){
                    curr_score+=20;
                    bubbles.push_back(Bubble(x, y, current_radius/2.0, -BUBBLE_DEFAULT_VX-30, current_vy, COLOR(191, 89, 253)));
                    bubbles.push_back(Bubble(x, y, current_radius/2.0, BUBBLE_DEFAULT_VX+30, current_vy, COLOR(191, 89, 253)));
                }
                else if (current_radius==2*BUBBLE_DEFAULT_RADIUS) {
                    curr_score+=40;
                    bubbles.push_back(Bubble(x, y, current_radius/2.0, -BUBBLE_DEFAULT_VX-50, current_vy, COLOR(53, 59, 253)));
                    bubbles.push_back(Bubble(x, y, current_radius/2.0, BUBBLE_DEFAULT_VX+50, current_vy, COLOR(53, 59, 253)));
                }
                else {
                    curr_score+=50;
                }
            }
        }
    }
    
}

bool collision_shooter_head(vector<Bubble> &bubbles,int x_shooter,int y_shooter,int shooter_radius){
    //we will check if the bubble hits the shooter head
    for (unsigned int i=0;i<bubbles.size();i++){
        double distance = pow(pow(bubbles[i].get_center_x()-x_shooter,2)+pow(bubbles[i].get_center_y()-y_shooter,2),0.5);
        if (distance<bubbles[i].get_radius()+shooter_radius){
            return true;
        }   
    }
    return false;
}

bool collision_shooter_body(vector<Bubble> &bubbles, int x_shooter, int y_shooter, int width, int height){
    // we will check if the shooter body hits the bubble
    for (unsigned int i=0;i<bubbles.size();i++){
        bool xcheck=(bubbles[i].get_center_x()-bubbles[i].get_radius()<x_shooter+width/2.0) && (bubbles[i].get_center_x()+bubbles[i].get_radius()>x_shooter-width/2.0);
        bool ycheck= (bubbles[i].get_center_y()-bubbles[i].get_radius()<y_shooter+height/2.0) && (bubbles[i].get_center_y()+bubbles[i].get_radius()>y_shooter-height/2.0);
        if (xcheck&&ycheck){
            return true;
        }  
    }
    return false;
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS*4, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    return bubbles;
}

//creating the healthbar
vector <Health> create_heathbar(){

    //set up the health bar
    vector <Health> hp;
    hp.push_back(Health(423,20,12,20,COLOR(247, 20, 14)));
    hp.push_back(Health(436,20,12,20,COLOR(255, 0,0)));
    hp.push_back(Health(449,20,12,20,COLOR(254, 168, 0)));
    hp.push_back(Health(462,20,12,20,COLOR(254, 242, 0)));
    hp.push_back(Health(475,20,12,20,COLOR(97, 229, 75)));
    hp.push_back(Health(488,20,12,20,COLOR(96, 242, 0)));
    return hp;
}

void clear_bubbles_and_bullets(vector <Bubble> &bubbles, vector <Bullet> &bullets){
    //to clear all bubbles and bullets on the screen once we have finished the game
    bubbles.clear();
    bullets.clear();
}

bool immunity(int &t, bool &use_immunity, Shooter &shooter){
    //this function provides immunity to the shooter for a short time when the bubble is passing through the shooter
    //the way this work is that we change the value of use_immunity a boolean to false when we no longer want the immunity to be used
    if (use_immunity){
        if(t<35){
            t++;
            return false;//by returning a false we make sure that the if condition down below wont keep reducing the health
        }
        else {
            use_immunity=false;
            t=0;
            return true;
        }
    }
    else {
        return true;
    }
}
void start_page(){
    //A start page of sorts
    Rectangle start_screen(250,250,500,500);
    start_screen.setColor(COLOR(221,230,125));
    start_screen.setFill(true);
    Text Start(250,250,"START GAME");
    Text rules(250,300,"You have 100 seconds. Good Luck!");
    wait(3);
    rules.hide();
    Start.hide();
    //countdown to start
    Text Number3(250,230,"3");
    wait(1);
    Number3.hide();
    Text Number2(250,230,"2");
    wait(1);
    Number2.hide();
    Text Number1(250,230,"1");
    wait(1);
    Number1.hide();

    start_screen.setFill(false);
}

int main()
{
    //opening the canvas 
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);
    //calling the start page fucntion
    start_page();

    //actual game starts
    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));
    
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    //putting the text for score
    stringstream curr_score_string_stream;
    int curr_score=0;
    string score_string("Score: ");
    Text score_text_string(LEFT_MARGIN, TOP_MARGIN, score_string);

    //putting the text saying health
    Text Health_bar_intext(370,20,"Health: ");

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    //initialize the healthbar
    vector <Health> healthpoint= create_heathbar();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    //buffer time for immunity
    int time_for_immunity=0;
    bool use_immunity=false;

    //a timer 
    int current_time=0;
    stringstream curr_time_ss;
    string time_string("Time: ");
    Text time_object(LEFT_MARGIN+150,BOTTOM_MARGIN,time_string);

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        //calling the collision functions which checks for the bullet and bubble collision
        collision(bubbles,bullets,curr_score);

        //printing out the score at any instant
        curr_score_string_stream.clear();
        curr_score_string_stream<<curr_score;
        string score_to_print;
        curr_score_string_stream>>score_to_print;
        score_string="Score :";
        score_string+=score_to_print;
        score_text_string.setMessage(score_string);

        //printing out the time at any instant
        current_time+=1;
        curr_time_ss.clear();
        //for the time left to be precise
        curr_time_ss<<(3000-current_time)/30;
        string time_string_withchanges;
        curr_time_ss>>time_string_withchanges;
        time_string="Time: ";
        time_string+=time_string_withchanges;
        time_object.setMessage(time_string);


        
        //reducing health on shooter hitting the bubble
        bool is_collision_with_head_bubble=collision_shooter_head(bubbles,shooter.get_head_center_x(),shooter.get_head_center_y(),shooter.get_head_radius());
        bool is_collision_with_body_bubble=collision_shooter_body(bubbles,shooter.get_body_center_x(),shooter.get_body_center_y(),shooter.get_body_width(),shooter.get_body_width());
        if ((is_collision_with_body_bubble||is_collision_with_head_bubble)&&immunity(time_for_immunity,use_immunity,shooter)){
            remove_health(healthpoint,healthpoint.size()-1);
            //on changing the boolean use_immunity we will change the part such that the immunity fucntion only retuurns false cause then 
            //we wont reduce the health multiple times with a single bubble collision
            //by defauly we keep the immunity function to return true when we arent having immunity enabled since anyways
            //we have an and operation so only if there is an actual collsion will the if condition be made true
            use_immunity=true;
        }

        //for stopping/ ending the game
        if (healthpoint.size()==0|| bubbles.size()==0||current_time>3000){
            clear_bubbles_and_bullets(bubbles,bullets);
            break;
        }


        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        wait(STEP_TIME);
    }

    //final game over screen
    Rectangle final_screen(250,250,500,500);
    final_screen.setColor(COLOR(247,169,169));
    final_screen.setFill(true);
    Text game_over_text1(250,250,"GAME OVER");
    Text game_over_text2(250,280,"Click anywhere to exit");
    
    getClick();
}
