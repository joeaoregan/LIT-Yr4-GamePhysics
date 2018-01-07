/*
//Using SDL, SDL OpenGL, standard IO, and, strings
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();										// Starts up SDL, creates window, and initializes OpenGL

bool initGL();										// Initializes matrices and clear color

void handleKeys(unsigned char key, int x, int y);	// Input handler

void update();										// Per frame update

void render();										// Renders quad to the screen

void close();										// Frees media and shuts down SDL

SDL_Window* gWindow = NULL;							// The window we'll be rendering to

SDL_GLContext gContext;								// OpenGL context

bool gRenderQuad = true;							// Render flag

bool init() {	
	bool success = true; // Initialization flag

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL) {
				printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0) {
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				// Initialize OpenGL
				if (!initGL())  {
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL() {
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Check for error
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Check for error
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	
	glClearColor(0.f, 0.f, 0.f, 1.f);	// Initialize clear color

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	return success;
}

void handleKeys(unsigned char key, int x, int y) {
	//Toggle quad
	if (key == 'q')	{
		gRenderQuad = !gRenderQuad;
	}
}

void update() {
	//No per frame update needed
}

void render() {	
	glClear(GL_COLOR_BUFFER_BIT);	//Clear color buffer

	//Render quad
	if (gRenderQuad) {
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
	}
}

void close() {
	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
		
	SDL_Quit();		// Quit SDL subsystems
}

int main(int argc, char* args[]) {
	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {		
		bool quit = false;		// Main loop flag
				
		SDL_Event e;			// Event handler
				
		SDL_StartTextInput();	// Enable text input

		// While application is running
		while (!quit) {
			// Handle events on queue
			while (SDL_PollEvent(&e) != 0) {
				// User requests quit
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				// Handle keypress with current mouse position
				else if (e.type == SDL_TEXTINPUT) {
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					handleKeys(e.text.text[0], x, y);
				}
			}
						
			render();	//Render quad
						
			SDL_GL_SwapWindow(gWindow);	//Update screen
		}
				
		SDL_StopTextInput();	//Disable text input
	}
	
	close();	//Free resources and close SDL

	return 0;
}
*/