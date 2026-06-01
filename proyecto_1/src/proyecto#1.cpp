#include "Engine2D.h"
#include "line.h"
#include "canvas.h"
#include "quadTree.h"
#include "triangle.h"
#include "ellipse.h"
#include <iostream>

class proyecto1 : public Engine2D {
    Canvas canvas;
    quadTree tree;
    Color colorFondo = Color(0.1f, 0.1f, 0.15f);
    Color borderColor = Color(1.0f, 0.0f, 0.0f);
    Color fillColor = Color(1.0f, 0.0f, 0.0f);
    bool showTree = true;
    enum BrushStates {
        DRAWING,
        SELECTING,
        IDLE
    };
    int state = IDLE;

    enum BrushTypes {
        LINE,
        RECTANGLE,
        TRIANGLE,
        ELLIPSE,
        NONE
      };
    int brush = LINE;
    int currentResize = LINE;
    bool filling = false;
    Shape* selectedShape = nullptr;
    std::vector<Shape*> lastShapeHits;
    size_t selectionCycleIndex = 0;

public:
    proyecto1(): Engine2D(1024, 600, "Proyecto #1 - Gestion y Despliegue de Primitivas"),
        canvas([this](int x, int y, const Color& color) { putPixel(x, y, color); }),
        tree(Point(0, 0), Point(1023, 599), [this](int x, int y, const Color& color) { putPixel(x, y, color); }) {}
    void setup() override {
        clear(colorFondo);
        std::cout << "Motor inicializado exitosamente." << std::endl;

        }

    // Eventos
    void onkeyDown(int key) override {
        if (key == GLFW_KEY_SPACE) {
            clear(colorFondo);
            std::cout << brush << std::endl;
        }
    }

    void onMouseButtonDown(const int button, const double x, const double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (ImGui::GetIO().WantCaptureMouse) {
                return;
            }
            if (state == SELECTING) {
                const std::vector<Shape*> shapeHits = tree.getShapesByLeaf(Point(static_cast<int>(x), static_cast<int>(y)));
                if (shapeHits.empty()) {
                    selectedShape = nullptr;
                    lastShapeHits.clear();
                    selectionCycleIndex = 0;
                    return;
                }

                if (shapeHits == lastShapeHits) {
                    selectionCycleIndex = (selectionCycleIndex + 1) % shapeHits.size();
                } else {
                    lastShapeHits = shapeHits;
                    selectionCycleIndex = 0;
                }

                // Cycle from topmost to bottom-most shape in the same hit pool
                const size_t selectedIndex = lastShapeHits.size() - 1 - selectionCycleIndex;
                selectedShape = lastShapeHits[selectedIndex];
                return;
            }
            if (state == IDLE) {
                selectedShape = nullptr;
                lastShapeHits.clear();
                selectionCycleIndex = 0;
                state = DRAWING;
                switch (brush) {
                    case LINE:
                        currentResize = LINE;
                        canvas.addLine(Point(static_cast<int>(x), static_cast<int>(y)), borderColor);
                        break;
                    case RECTANGLE:
                        currentResize = RECTANGLE;
                        canvas.addRectangle(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;
                    case TRIANGLE:
                        currentResize = TRIANGLE;
                        canvas.addTriangle(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;
                    case ELLIPSE:
                        currentResize = ELLIPSE;
                        canvas.addEllipse(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;

                }
            }
        }
    }


    void onMouseButtonUp(int button, double x, double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (state == DRAWING) {
                state = IDLE;
                currentResize = NONE;
            }
        }
    }
    // Evento de movimiento continuo
    void onMouseMove(double x, double y) override {
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        if (state == DRAWING) {
            int ix = static_cast<int>(x);
            int iy = static_cast<int>(y);
            switch (currentResize) {
                case NONE:
                    break;
                case LINE:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeLine(Point(ix, iy), latestShape);
                    }
                    break;
                case RECTANGLE:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeRectangle(Point(ix, iy), latestShape);
                    }
                    break;
                case TRIANGLE:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeTriangle(Point(ix, iy), latestShape);
                    }
                    break;
                case ELLIPSE:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeEllipse(Point(ix, iy), latestShape);
                    }
                    break;


            }
        }

    }
    void update(float deltaTime) override {
        clear(colorFondo);
        canvas.draw();
        if (selectedShape != nullptr) {
            canvas.highLightShape(selectedShape);
        }
        tree.rebuild(canvas.getShapes());
        if (showTree){tree.draw();}
    }
    void drawUI() override {
        ImGui::Begin("Herramientas");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        float borderCol[3] = { borderColor.r, borderColor.g, borderColor.b };
        if (ImGui::ColorEdit3("Border Color", borderCol)) {
            borderColor.r = borderCol[0];
            borderColor.g = borderCol[1];
            borderColor.b = borderCol[2];
        }

        float fillCol[3] = { fillColor.r, fillColor.g, fillColor.b };
        if (ImGui::ColorEdit3("Fill Color", fillCol)) {
            fillColor.r = fillCol[0];
            fillColor.g = fillCol[1];
            fillColor.b = fillCol[2];
        }
        ImGui::Text("Manten click izquierdo para dibujar.");
        ImGui::Text("Presiona ESPACIO para limpiar.");
        ImGui::RadioButton("Line", &brush, LINE); ImGui::SameLine();
        ImGui::RadioButton("Rectangle", &brush, RECTANGLE); ImGui::SameLine();
        ImGui::RadioButton("Triangle", &brush, TRIANGLE);ImGui::SameLine();
        ImGui::RadioButton("Ellipse", &brush, ELLIPSE);
        ImGui::Checkbox("Filling", &filling);
        ImGui::Checkbox("Show Tree", &showTree);
        ImGui::RadioButton("Draw", &state, IDLE);ImGui::SameLine(); //So it can wait to draw a shape
        ImGui::RadioButton("Selection", &state, SELECTING);
        ImGui::End();
    }
};

int main() {
    proyecto1 app;
    app.run();
    return 0;
}