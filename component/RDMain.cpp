#include "../raylibLibrary/raylib.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <thread>   // For sleep
#include <chrono>   // For sleep duration
// #include "raylib.h"
#include "Resistor.hpp"
#include "Diode.hpp"

using namespace std;

const double T = 0.0000001; // timestep of 0.1uS
const double timeMax = 0.02;
const double freq = 50;    // 50Hz
const double Vpeak = 10.0; // 10V
const int NUM_COMPONENTS = 2;

void CostFunctionV(Component *component[], const int NUM, double &current, const double Is, const double voltage);

int main()
{
    double width, height, scalingFactor;

    cout << "Enter the width of your display: ";
    cin >> width;
    cout << "Enter the height of your display: ";
    cin >> height;
    cout << "Enter the height scaling factor: ";
    cin >> scalingFactor;

    // Initialize Raylib
    InitWindow(static_cast<int>(width), static_cast<int>(scalingFactor * height), "RDCircuit");
    
    // Check if the window is initialized successfully
    if (!IsWindowReady()) {
        cerr << "Failed to initialize Raylib window!" << endl;
        return 1;
    }
    
    SetTargetFPS(60); // Set the target frames per second

    Component *component[NUM_COMPONENTS];
    // 1 micro-amp reverse saturation current, room temperature of 290K or 17C, emission coefficient of 2 for a silicon diode
    double Is = 0.000001;
    double Vt = 290.0 / 11586.0;
    double n = 2.0;
    component[0] = new Diode(Is, Vt, n);
    // 100 ohm resistor
    component[1] = new Resistor(100.0);

    // Run the simulation
    double I = 0.0;

    while (!WindowShouldClose()) // While the window is open
    {
        BeginDrawing();  // Start drawing
        ClearBackground(RAYWHITE);  // Clear the window with white background

        // Draw a horizontal line as a baseline
        DrawLine(0, static_cast<int>(scalingFactor * height / 2.0), static_cast<int>(width), static_cast<int>(scalingFactor * height / 2.0), BLACK);

        for (double time = 0.0; time < timeMax; time += T)
        {
            double V = Vpeak * sin(2.0 * PI * freq * time);
            // plot the applied voltage in red
            double xpos = time * width / timeMax;
            double ypos = (height * V / Vpeak) / 2.0 + scalingFactor * height / 2.0;
            DrawPixel(static_cast<int>(xpos), static_cast<int>(ypos), RED);

            // determine the voltages across the components and plot them in green
            CostFunctionV(component, NUM_COMPONENTS, I, Is, V); // pass through the previous I, CostFunction will return the new I
            cout << "The applied voltage is " << V << "V." << endl;
            cout << "The current is " << I << "A." << endl;

            for (int i = 0; i < NUM_COMPONENTS; ++i)
            {
                ypos = (height * (component[i]->GetVoltage(I)) / Vpeak) / 2.0 + scalingFactor * height / 2.0;
                if (i == 0)
                    DrawPixel(static_cast<int>(xpos), static_cast<int>(ypos), GREEN);
                else
                    DrawPixel(static_cast<int>(xpos), static_cast<int>(ypos), BLUE);
                component[i]->Describe();
            }

            // Delay for visual effect using std::this_thread::sleep_for
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Delay for 1ms
        }

        EndDrawing(); // Finish drawing
    }

    CloseWindow(); // Close window and OpenGL context

    cout << "DONE!" << endl;
    return 0;
}

void CostFunctionV(Component *component[], const int NUM, double &current, const double Is, const double voltage)
{
    double I1 = current;
    double J1 = 0.0;
    double J0 = 0.0;
    double alpha = 1.0;
    const double tolerance = 0.001;

    do
    {
        double sumVoltage = 0.0;
        for (int i = 0; i < NUM; ++i)
        {
            double Vcomponent = component[i]->GetVoltage(I1);
            sumVoltage += Vcomponent;
        }
        J1 = sumVoltage - voltage;

        // Reduce alpha if J1 and J0 are of opposite signs or are equal to each other
        if ((abs(J0 - J1) != (J0 - J1)) || J0 == J1)
            alpha /= 2.0;

        if (abs(J1) > tolerance)
        {
            if (J1 < 0)
            { // increase the current
                I1 += alpha;
            }
            else
            {
                I1 -= alpha;
                if (I1 < -Is)
                { // corner-case, the reverse saturation current of the diode
                    I1 = -Is;
                }
            }
            J0 = J1;
        }
        if (alpha < tolerance / 1000000.0)
        { // reset alpha, skip!
            // alpha = ((double)rand() / (RAND_MAX)) + 0.5;//between 0.5 and 1.5
        }
    } while (abs(J1) > tolerance);

    current = I1;
}
