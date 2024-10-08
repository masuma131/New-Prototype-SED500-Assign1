#include <raylibLibrary/raylib.h>
// #include <raylib.h>
#include <cmath>    // For sin function
#include <vector>   // For storing points
#include <iostream>

// Constants for the graph
const int screenWidth = 1500;  // Width of the window
const int screenHeight = 700;  // Height of the window

const float Vpeak = 13.0f;      // Peak voltage for the first wave (0 to peak)
const float freq = 3.0f;        // Frequency for the first wave in Hz
const int samples = 2000;       // Number of samples for the first sine wave

const float Vpeak2 = 2.0f;      // Peak voltage for the second wave (0 to peak)
const float freq2 = 3.0f;       // Reduced frequency for the second wave in Hz
const int samples2 = 2000;      // Number of samples for the second sine wave

int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Total voltage graph");
    SetTargetFPS(65);

    // Variables to control the drawing process
    int currentSample = 0;  // Keep track of the current sample drawn
    float offset = 5;       // X-axis offset for the second wave

    std::vector<Vector2> points;  // Vector to store points for the first wave
    std::vector<Vector2> points2; // Vector to store points for the second wave

    while (!WindowShouldClose())
    {
        // Draw
        BeginDrawing();
        ClearBackground(GRAY);

        // Draw the axes
        DrawLine(50, screenHeight / 2, screenWidth - 50, screenHeight / 2, BLACK); // X-axis
        DrawLine(50, 50, 50, screenHeight - 50, BLACK); // Y-axis

        // Incrementally draw the sine wave graphs
        if (currentSample < samples)
        {
            // Calculate the x and y coordinates for the first sine wave
            float x = 50 + currentSample * (screenWidth - 100) / samples;
            float y = (screenHeight / 2) - (Vpeak * sin((2 * PI * freq * currentSample / samples))) * (screenHeight / 2 - 50) / Vpeak;
           
            // Calculate the x and y coordinates for the second sine wave with offset and different frequency
            float x2 = 50 + currentSample * (screenWidth - 100) / samples + offset; // Using currentSample for both
            float y2 = (screenHeight / 2) - (Vpeak2 * sin((2 * PI * freq2 * currentSample / samples))) * (screenHeight / 2 - 50) / Vpeak2;

            // Store the points for the trace
            points.push_back({ x, y });
            points2.push_back({ x2, y2 });

            // Draw the traces
            for (size_t i = 1; i < points.size(); i++)
            {
                DrawLine(points[i - 1].x, points[i - 1].y, points[i].x, points[i].y, GREEN);
                DrawLine(points2[i - 1].x, points2[i - 1].y, points2[i].x, points2[i].y, RED);
            }

            // Draw the current points
            DrawCircle(x, y, 4, GREEN);
            DrawCircle(x2, y2, 3, RED);

            // Increment the sample counter
            currentSample++;
        }

        // Display information
        // DrawText("Total Voltage Graph with Trace", 10, 10, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
