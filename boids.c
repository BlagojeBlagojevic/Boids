#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDl_timer.h>
#include<assert.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#undef main                //275 183


#define width  400
#define height 400
#define Nboids 500

typedef struct BOIDS{
	
	int positions[2];
	int speed[2];
	int acceleration[2];
	uint8_t color[4];
	
	
}BOIDS;

float rand_neg(){
	//return 0;
	if(rand()%2==0)
		return -1.0*(rand()%10);
	return rand()%10;
}
void RandomBoids(BOIDS *boids)
{
	srand(time(0));
	//srand(10000);
	
	for(size_t i = 0; i < Nboids; i++){
		boids[i].positions[0] = (rand())%width;
		boids[i].positions[1] = (rand())%height;
		boids[i].acceleration[0] = 0;
		boids[i].acceleration[1] = 0;
		boids[i].speed[0] = rand_neg();
		boids[i].speed[1] = rand_neg();
		boids[i].color[0]= rand()%255;
		boids[i].color[1]= rand()%255;
		boids[i].color[2]= rand()%255;
		boids[i].color[3]= rand()%255;
	}
}

void DrawBoids(SDL_Renderer *renderer,BOIDS *boids){
	
	
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	for(size_t i = 0; i < Nboids; i++){
		SDL_SetRenderDrawColor(renderer,boids[i].color[0],boids[i].color[1],boids[i].color[2],boids[i].color[3]);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0],boids[i].positions[1]);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0]+1,boids[i].positions[1]);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0],boids[i].positions[1]+1);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0]+1,boids[i].positions[1]+1);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0],boids[i].positions[1]);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0]+2,boids[i].positions[1]);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0],boids[i].positions[1]+2);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0]+2,boids[i].positions[1]+2);
		SDL_RenderDrawPoint(renderer,boids[i].positions[0],boids[i].positions[1]);
	}
	SDL_SetRenderDrawColor(renderer,0,0,0,0);
	
	SDL_RenderPresent(renderer);
}

void MoveTowardCenterOfMass(BOIDS *boids)
{
	float Cmass[2];
	
	Cmass[0]=0;
	Cmass[1]=0;
	for(size_t i = 0; i < Nboids; i++){
		Cmass[0]+=boids[i].positions[0];
		Cmass[1]+=boids[i].positions[1];
	}
	Cmass[0]/=(Nboids);
	Cmass[1]/=(Nboids);
	//printf("C1 %d C2 %d\n",Cmass[0],Cmass[1]);
	//system("pause");
	
	float factor = 100;  //How mutch move toward a center of mass
	
		for(size_t i = 0; i < Nboids; i++){
		boids[i].speed[0]=(int)Cmass[0] / factor+rand_neg();
		boids[i].speed[1]=(int)Cmass[1] / factor-rand_neg();
			
	}
		
	
}
void CheckNearest(BOIDS *boids){
	for(size_t i = 0 ; i < Nboids; i++){
		for(size_t j = 0 ; j < Nboids; j++){	
			if(abs(boids[i].positions[0] - boids[j].positions[0]) <= 5 ){
				boids[i].positions[0]-=(boids[i].positions[0] - boids[j].positions[0]);
			}
			
			if(abs(boids[i].positions[1] - boids[j].positions[1]) <= 5 ){
				boids[i].positions[1]-=(boids[i].positions[1] - boids[j].positions[1]);
			}
	
	
	}
	
	}
}

void UpdateSpeed(BOIDS *boids){
	int Cmass[2];
	
	Cmass[0]=0;
	Cmass[1]=0;
	for(size_t i = 0; i < Nboids; i++){
		Cmass[0]+=boids[i].speed[0];
		Cmass[1]+=boids[i].speed[1];
	}
	Cmass[0]/=(Nboids);
	Cmass[1]/=(Nboids);
	//printf("C1 %d C2 %d\n",Cmass[0],Cmass[1]);
	//system("pause");
	
	float factor = 8;  //How mutch move toward a center of mass
	
		for(size_t i = 0; i < Nboids; i++){
		boids[i].speed[0]=(int)Cmass[0] / factor+rand_neg();
		boids[i].speed[1]=(int)Cmass[1] / factor+rand_neg();
			
	}
		
	

}

void BoidsUpdate(BOIDS *boids){
	
		MoveTowardCenterOfMass(boids);
		CheckNearest(boids);
		UpdateSpeed(boids);
		
		
		for(size_t i = 0; i < Nboids; i++){
		if(boids[i].speed[0]>10)
			boids[i].speed[0]=10;
		if(boids[i].speed[1]>10)
			boids[i].speed[1]=10;
			
		boids[i].positions[1]+=boids[i].speed[1];
		boids[i].positions[0]+=boids[i].speed[0];
		if(boids[i].positions[0] > width)
			{
				
				boids[i].positions[0] = 0;
				//boids[i].speed[0]=0;
				
			} 
					
		if(boids[i].positions[1] > height)
		{
				boids[i].positions[1] = 0;
				//boids[i].speed[1]=0;
		}
		
		if(boids[i].positions[1] < 0)
		{
				boids[i].positions[1] = height-10;
				//boids[i].speed[1]=0;
		}
		if(boids[i].positions[0] < 0)
		{
				boids[i].positions[0] = width-10;
				//b//oids[i].speed[0] = 0;
		}	
		
		
										  }
}



int main()
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	SDL_CreateWindowAndRenderer(width,height,NULL,&window,&renderer);
	
	
	BOIDS boids[Nboids];
	RandomBoids(&boids);
	while(1){
		DrawBoids(renderer,&boids);
		
		BoidsUpdate(&boids);
		//SDL_Delay(1);
		//system("pause");
	}
	DrawBoids(renderer,&boids);
	system("pause");
	
	
	printf("Nesto");
	return 0;
}
