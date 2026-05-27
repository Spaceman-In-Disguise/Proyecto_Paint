#include "Engine2D.h"
#include "line.h"
#include "canvas.h"
#include <iostream>

class proyecto1 : public Engine2D {
private:
    Canvas canvas;
    Color colorFondo = Color(0.1f, 0.1f, 0.15f);
    Color colorPincel = Color(1.0f, 0.0f, 0.0f);
    bool dibujando = false;
    enum BrushTypes {
        LINE,
        TRIANGLE,
        RECTANGLE,
        CIRCLE
      };
    enum BrushTypes brush = LINE;

    const Line::PixelCallback pixelWriter;

public:
    proyecto1(): Engine2D(1024, 600, "Proyecto #1 - Gestion y Despliegue de Primitivas"),
        pixelWriter([this](int x, int y, const Color& color) { putPixel(x, y, color); }) {}
    void setup() override {
        clear(colorFondo);
        std::cout << "Motor inicializado exitosamente." << std::endl;
        canvas.addShape(std::make_unique<Line>(
                Point(80,80),
                Point(1000,1000),
                colorPincel,
                pixelWriter
            )
        );
        canvas.draw();
    }
    // Eventos
    void onkeyDown(int key) override {
        if (key == GLFW_KEY_SPACE) {
            clear(colorFondo);
        }
    }
    void onMouseButtonDown(int button, double x, double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            dibujando = true;
        }
        putPixel(x, y, colorPincel);
    }
    void onMouseButtonUp(int button, double x, double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            dibujando = false;
        }
    }
    // Evento de movimiento continuo
    void onMouseMove(double x, double y) override {
        if (dibujando) {
            int ix = static_cast<int>(x);
            int iy = static_cast<int>(y);
            putPixel(ix, iy, colorPincel);
        }
    }
    void update(float deltaTime) override {
        // Aquí irían cosas que cambian automáticamente con el tiempo
    }
    void drawUI() override {
        ImGui::Begin("Herramientas");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        float col[3] = { colorPincel.r, colorPincel.g, colorPincel.b };
        if (ImGui::ColorEdit3("Color Pincel", col)) {
            colorPincel.r = col[0];
            colorPincel.g = col[1];
            colorPincel.b = col[2];
        }
        ImGui::Text("Manten click izquierdo para dibujar.");
        ImGui::Text("Presiona ESPACIO para limpiar.");
        ImGui::End();
    }
};

int main() {
    proyecto1 app;
    app.run();
    return 0;
}