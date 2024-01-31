#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;
double func     (double x){return 3*pow(x,2);}
double derFunc  (double x){return 6*x;}
int range[2]{-5,5};
float resolution{.25f};
struct Vertex {
    vector<double>   vertexPos{0};
    vector<double>   vertexRGB{0}; //RGB
    double           derVertex{0};
} ;

void SpiralingData();

Vertex vert;
vector<Vertex> vertices;
int main() {
    double x = range[0];
    double xi{0};
    double dx{0};
    //Write to file
    ofstream fileToWriteTo;
    //todo Vær sikker på at det er en folder i c:/ som heter data, hvis folderen ikke eksisterer må *DU* opprette folderen
    fileToWriteTo.open("c:/data/VertexData.txt", ios::trunc);
    //Write amount of indices
    int indicesAmount = ((range[1]-range[0])/resolution)+1;
    fileToWriteTo << indicesAmount*6 << endl; // to account for 3 separate pos values, and 3 color values
    fileToWriteTo <<  fixed << setprecision(2);
    for (int i = 0; i < indicesAmount; ++i) {
        xi = func(x);
        dx = derFunc(x);
        vert.vertexPos = {x, xi};
        vert.derVertex=dx;
        vertices.push_back(vert);
        x += resolution;
    }

    for (int i = 0; i < vertices.size(); ++i) {
        if(vertices.at(i).derVertex > 0){ // green when positive
            vertices.at(i).vertexRGB = {.0f, 1.f, .0f};
        }
        else if (vertices.at(i).derVertex < 0){ // red when negative
            vertices.at(i).vertexRGB = {1.f, .0f, .0f};
        }
        else vertices.at(i).vertexRGB = {.0f, .0f, 1.f}; // blue at 0 point
        for (int j = 0; j < vertices.at(i).vertexPos.size(); ++j) {
            fileToWriteTo << vertices.at(i).vertexPos.at(j) << endl;
        }
        fileToWriteTo << 0 << endl; // the 3rd dimension
        for (int j = 0; j < vertices.at(i).vertexRGB.size(); ++j) {
            fileToWriteTo << vertices.at(i).vertexRGB.at(j) << endl;
        }
        cout << "x = "<<vertices.at(i).vertexPos[0] <<" y = "<<  vertices.at(i).vertexPos[1]
        << "  Derived = " << vertices.at(i).derVertex << endl;
    }
    fileToWriteTo.close();
    SpiralingData();
    return 0;
}

void SpiralingData() {
    ofstream fileToWriteTo;
    fileToWriteTo.open("c:/data/CircleData.txt", ios::trunc);

    int fullCircles = 5;
    int indicesPerCircle = 90;
    int totalIndicesAmount = indicesPerCircle*fullCircles;
    float angleDiff = 360/indicesPerCircle;
    float curAngle = 0;

    float startRadius{1};
    float endRadius{0.1f};
    float radius = startRadius;
    fileToWriteTo << totalIndicesAmount << endl;

    for (int i = 0; i < totalIndicesAmount; ++i) {
        radius -= (startRadius - endRadius)/totalIndicesAmount;
        curAngle += angleDiff;
        float xVal = 0 + radius * cos(curAngle);
        float yVal = 0 + radius * sin(curAngle);
        fileToWriteTo << xVal << endl;
        fileToWriteTo << yVal << endl;
    }
    fileToWriteTo.close();
}
