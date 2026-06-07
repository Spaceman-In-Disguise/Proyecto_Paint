#include "Engine2D.h"
#include "line.h"
#include "canvas.h"
#include "quadTree.h"
#include "triangle.h"
#include "ellipse.h"
#include "editor_memento.h"
#include "history_manager.h"
#include "save_manager.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

class proyecto1 : public Engine2D {
    Canvas canvas;
    quadTree tree;
    Color backgroundColor = Color(0.1f, 0.1f, 0.15f);
    Color borderColor = Color(1.0f, 0.0f, 0.0f);
    Color fillColor = Color(1.0f, 0.0f, 0.0f);
    bool showTree = true;
    enum BrushStates {
        DRAWING,
        SELECTING,
        EDITING,
        IDLE
    };
    int state = IDLE;
    bool isEditing = false;

    enum BrushTypes {
        LINE,
        RECTANGLE,
        TRIANGLE,
        ELLIPSE,
        BEZIER,
        NONE
      };
    int brush = LINE;
    int currentResize = LINE;
    bool filling = false;
    Shape* selectedShape = nullptr;
    std::vector<Shape*> lastShapeHits;
    size_t selectionCycleIndex = 0;
    bool isDragging = false;
    bool isDraggingHandle = false;
    int activeControlPointIndex = -1;
    int dragLastX = 0;
    int dragLastY = 0;

    double lastClickTime = 0.0;
    const double doubleClickThreshold = 0.3; // 0.3 seconds (300ms)

    //UNDO - REDO Logic
    HistoryManager history;

    std::shared_ptr<EditorMemento> createMemento() {
        std::vector<std::unique_ptr<Shape>> clonedShapes;
        for (Shape* shape : canvas.getShapes()) {
            clonedShapes.push_back(shape->clone());
        }
        return std::make_shared<EditorMemento>(std::move(clonedShapes), backgroundColor);
    }

    void restoreFromMemento(std::shared_ptr<EditorMemento> memento) {
        if (!memento) return;

        backgroundColor = memento->getBackgroundColor();

        std::vector<std::unique_ptr<Shape>> restoredShapes;
        for (const auto& shapePtr : memento->getShapes()) {
            restoredShapes.push_back(shapePtr->clone());
        }
        canvas.setShapes(std::move(restoredShapes));

        selectedShape = nullptr;
        state = IDLE;
        isEditing = false;
    }

    void saveState() {
        history.save(createMemento());
    }
    char paintingFilePath[260] = "painting.txt";

    bool savePaintingToFile(const std::string& filePath) const {
        return SaveManager::savePainting(filePath, canvas.getShapes(), backgroundColor);
    }
    bool loadPaintingFromFile(const std::string& filePath) {
        std::vector<SavedShape> loadedShapes;
        Color loadedBackground = backgroundColor;

        if (!SaveManager::loadPainting(filePath, loadedShapes, loadedBackground)) {
            return false;
        }

        saveState();
        backgroundColor = loadedBackground;
        canvas.clearAll();

        // Reconstruct the canvas using data read by the SaveManager
        for (const auto& record : loadedShapes) {
            if (record.points.empty()) continue;

            switch (record.type) {
                case SavedShapeType::Line: {
                    if (record.points.size() >= 2) {
                        if (Line* shape = canvas.addLine(record.points[0], record.borderColor)) {
                            shape->setP1(record.points[1]);
                            shape->borderColor = record.borderColor;
                            shape->fillColor = record.fillColor;
                            shape->fill = record.fill;
                        }
                    }
                    break;
                }
                case SavedShapeType::Rectangle: {
                    if (record.points.size() >= 2) {
                        if (Rectangle* shape = canvas.addRectangle(record.points[0], record.fill, record.borderColor, record.fillColor)) {
                            shape->setP1(record.points[1]);
                            shape->borderColor = record.borderColor;
                            shape->fillColor = record.fillColor;
                            shape->fill = record.fill;
                        }
                    }
                    break;
                }
                case SavedShapeType::Triangle: {
                    if (record.points.size() >= 3) {
                        if (Triangle* shape = canvas.addTriangle(record.points[0], record.fill, record.borderColor, record.fillColor)) {
                            shape->setP1(record.points[1]);
                            shape->setP2(record.points[2]);
                            shape->borderColor = record.borderColor;
                            shape->fillColor = record.fillColor;
                            shape->fill = record.fill;
                        }
                    }
                    break;
                }
                case SavedShapeType::Ellipse: {
                    if (record.points.size() >= 2) {
                        if (Ellipse* shape = canvas.addEllipse(record.points[0], record.fill, record.borderColor, record.fillColor)) {
                            shape->setP1(record.points[1]);
                            shape->borderColor = record.borderColor;
                            shape->fillColor = record.fillColor;
                            shape->fill = record.fill;
                        }
                    }
                    break;
                }
                case SavedShapeType::Bezier: {
                    if (record.points.size() >= 2) {
                        if (Bezier* shape = canvas.addBezier(record.points, record.borderColor)) {
                            shape->borderColor = record.borderColor;
                            shape->fillColor = record.fillColor;
                            shape->fill = record.fill;
                        }
                    }
                    break;
                }
            }
        }

        // Reset all application states
        selectedShape = nullptr;
        lastShapeHits.clear();
        selectionCycleIndex = 0;
        isDragging = false;
        isDraggingHandle = false;
        activeControlPointIndex = -1;
        state = IDLE;
        isEditing = false;
        currentResize = NONE;

        return true;
    }

    void performUndo() {
        if (auto previousState = history.undo(createMemento())) {
            restoreFromMemento(previousState);
        }
    }

    void performRedo() {
        if (auto nextState = history.redo(createMemento())) {
            restoreFromMemento(nextState);
        }
    }


    static bool isNearHandle(const Point& cursor, const Point& handle, int radius = 7) {
        const int dx = cursor.x - handle.x;
        const int dy = cursor.y - handle.y;
        return (dx * dx) + (dy * dy) <= radius * radius;
    }

    static void setControlPoint(Shape* shape, const int controlPointIndex, const Point& point) {
        if (auto line = dynamic_cast<Line*>(shape)) {
            if (controlPointIndex == 0) {
                line->setP0(point);
            } else if (controlPointIndex == 1) {
                line->setP1(point);
            }
            return;
        }

        if (auto rectangle = dynamic_cast<Rectangle*>(shape)) {
            if (controlPointIndex == 0) {
                rectangle->setP0(point);
            } else if (controlPointIndex == 1) {
                rectangle->setP1(point);
            }
            return;
        }

        if (auto triangle = dynamic_cast<Triangle*>(shape)) {
            if (controlPointIndex == 0) {
                triangle->setP0(point);
            } else if (controlPointIndex == 1) {
                triangle->setP1(point);
            } else if (controlPointIndex == 2) {
                triangle->setP2(point);
            }
            return;
        }

        if (auto ellipse = dynamic_cast<Ellipse*>(shape)) {
            if (controlPointIndex == 0) {
                ellipse->setP0(point);
            } else if (controlPointIndex == 1) {
                ellipse->setP1(point);
            }
        }
    }

    static int getNearbyControlPointIndex(Shape* shape, const Point& cursor, int radius = 7) {
        const std::vector<Point> controlPoints = Canvas::getControlPoints(shape);
        for (std::size_t index = 0; index < controlPoints.size(); ++index) {
            if (isNearHandle(cursor, controlPoints[index], radius)) {
                return static_cast<int>(index);
            }
        }

        return -1;
    }

public:
    proyecto1(): Engine2D(1024, 600, "Proyecto #1 - Gestion y Despliegue de Primitivas"),
        canvas([this](int x, int y, const Color& color) { putPixel(x, y, color); }),
        tree(Point(0, 0), Point(1023, 599), [this](int x, int y, const Color& color) { putPixel(x, y, color); }) {}
    void setup() override {
        clear(backgroundColor);
        std::cout << "Motor inicializado exitosamente." << std::endl;

        }

    // Events
    void onkeyDown(int key) override {

        if (const bool ctrlPressed = ImGui::GetIO().KeyCtrl; ctrlPressed && key == GLFW_KEY_Z) {
            performUndo();
        }
        else if (ctrlPressed && key == GLFW_KEY_Y) {
            performRedo();
        }
        else if (key == GLFW_KEY_SPACE) {
            saveState(); // Save before clearing everything
            clear(backgroundColor);
            std::cout << brush << std::endl;
        }
    }

    void onMouseButtonDown(const int button, const double x, const double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (ImGui::GetIO().WantCaptureMouse) {
                return;
            }

            double currentTime = glfwGetTime();
            bool isDoubleClick = (currentTime - lastClickTime) < doubleClickThreshold;
            lastClickTime = currentTime; // Reset the tracker

            const Point cursor(static_cast<int>(x), static_cast<int>(y));

            if (isDoubleClick) { //Edit Selected Shape
                if (state == SELECTING && selectedShape != nullptr && cursor.x > std::min(selectedShape->boundingBox[0].x, selectedShape->boundingBox[1].x) &&
                                     cursor.x < std::max(selectedShape->boundingBox[0].x, selectedShape->boundingBox[1].x) &&
                                     cursor.y > std::min(selectedShape->boundingBox[0].y, selectedShape->boundingBox[1].y) &&
                                     cursor.y < std::max(selectedShape->boundingBox[0].y, selectedShape->boundingBox[1].y)) {
                    isEditing = !isEditing;
                    state = isEditing ? EDITING : SELECTING;
                    return;
                }
            }
            if (state == EDITING && selectedShape != nullptr) {
                //Checks if cursor is outside the bounding box of the selected shape, if it is, we deselect the shape and reset the selection state
                if (cursor.x < std::min(selectedShape->boundingBox[0].x, selectedShape->boundingBox[1].x) - 10 ||
                                     cursor.x > std::max(selectedShape->boundingBox[0].x, selectedShape->boundingBox[1].x) + 10 ||
                                     cursor.y < std::min(selectedShape->boundingBox[0].y, selectedShape->boundingBox[1].y) - 10 ||
                                     cursor.y > std::max(selectedShape->boundingBox[0].y, selectedShape->boundingBox[1].y) + 10) {
                    selectedShape = nullptr;
                    state = SELECTING;
                    isEditing = false;
                    return;
                }
                activeControlPointIndex = getNearbyControlPointIndex(selectedShape, cursor);
                if (activeControlPointIndex >= 0) { // Moving handle to reshape
                    saveState();
                    isDragging = true;
                    isDraggingHandle = true;
                    dragLastX = cursor.x;
                    dragLastY = cursor.y;
                    return;
                }

                if (isNearHandle(cursor, selectedShape->position)) { //Moving whole shape
                    saveState();
                    isDragging = true;
                    isDraggingHandle = false;
                    dragLastX = cursor.x;
                    dragLastY = cursor.y;
                    return;
                }
            }
            if (state == SELECTING) {
                const std::vector<Shape*> shapeHits = tree.getShapesByLeaf(cursor);
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
                        saveState();
                        canvas.addLine(Point(static_cast<int>(x), static_cast<int>(y)), borderColor);
                        break;
                    case RECTANGLE:
                        currentResize = RECTANGLE;
                        saveState();
                        canvas.addRectangle(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;
                    case TRIANGLE:
                        currentResize = TRIANGLE;
                        saveState();
                        canvas.addTriangle(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;
                    case ELLIPSE:
                        currentResize = ELLIPSE;
                        saveState();
                        canvas.addEllipse(Point(static_cast<int>(x), static_cast<int>(y)), filling, borderColor, fillColor);
                        break;
                    case BEZIER:
                        currentResize = BEZIER;
                        saveState();
                        canvas.addBezier(Point(static_cast<int>(x), static_cast<int>(y)), borderColor);
                        break;

                }
            }
        }
    }


    void onMouseButtonUp(int button, double x, double y) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            isDragging = false;
            isDraggingHandle = false;
            activeControlPointIndex = -1;
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
        const int ix = static_cast<int>(x);
        const int iy = static_cast<int>(y);

        if (isDragging && selectedShape != nullptr) {
            if (isDraggingHandle) {
                setControlPoint(selectedShape, activeControlPointIndex, Point(ix, iy));
                return;
            }

            const int deltaX = ix - dragLastX;
            const int deltaY = iy - dragLastY;
            if (deltaX != 0 || deltaY != 0) {
                selectedShape->move(deltaX, deltaY);
                dragLastX = ix;
                dragLastY = iy;
            }
            return;
        }

        if (state == DRAWING) {
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
                case BEZIER:
                    if (Shape* latestShape = canvas.getLastShape(); latestShape != nullptr) {
                        Canvas::resizeBezier(Point(ix, iy), latestShape);
                    }
                    break;


            }
        }

    }
    void update(float deltaTime) override {
        clear(backgroundColor);
        canvas.draw();
        if (selectedShape != nullptr) {
            canvas.highLightShape(selectedShape);
            if (isEditing) {
                canvas.drawControlPoints(selectedShape);
            }
        }
        tree.rebuild(canvas.getShapes());
        if (showTree){tree.draw();}
    }
    void drawUI() override {
        ImGui::Begin("Herramientas");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        static char paintingPath[260] = "painting.txt";
        ImGui::InputText("Painting File", paintingPath, sizeof(paintingPath));
        if (ImGui::Button("Save Painting")) {
            savePaintingToFile(paintingPath);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Painting")) {
            loadPaintingFromFile(paintingPath);
        }
        ImGui::Separator();

        static bool isEditingBg = false;

        if (!ImGui::GetIO().MouseDown[0]) {
            isEditingBg = false;
        }

        float backgroundCol[3] = { backgroundColor.r, backgroundColor.g, backgroundColor.b };

        if (ImGui::ColorEdit3("Background Color", backgroundCol)) {
            if (!isEditingBg) {
                saveState();
                isEditingBg = true;
            }
            backgroundColor.r = backgroundCol[0];
            backgroundColor.g = backgroundCol[1];
            backgroundColor.b = backgroundCol[2];
        }

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
        ImGui::RadioButton("Bezier", &brush, BEZIER);
        ImGui::Checkbox("Filling", &filling);
        ImGui::Checkbox("Show Tree", &showTree);
        ImGui::RadioButton("Draw", &state, IDLE);ImGui::SameLine(); //So it can wait to draw a shape
        ImGui::RadioButton("Selection", &state, SELECTING);
        if (ImGui::Button("Clear All")) {
            canvas.clearAll();
        }
        ImGui::End();

        //Inspector for Selected Shape
        if (selectedShape != nullptr) {
            ImGui::Begin("Forma Seleccionada");
            ImGui::Separator();
            //Tell the user the type of the shape
            ImGui::Text("Tipo: %s", dynamic_cast<Line*>(selectedShape) ? "Linea" : dynamic_cast<Rectangle*>(selectedShape) ? "Rectangulo" : dynamic_cast<Triangle*>(selectedShape) ? "Triangulo" : dynamic_cast<Ellipse*>(selectedShape) ? "Elipse" : "Desconocida");
            //Set the color of the border and fill in the UI

            static bool isEditingBorder = false;
            if (!ImGui::GetIO().MouseDown[0]) {
                isEditingBorder = false;
            }

            float borderCol[3] = { selectedShape->borderColor.r, selectedShape->borderColor.g, selectedShape->borderColor.b };

            if (ImGui::ColorEdit3("Border Color", borderCol)) {
                if (!isEditingBorder) {
                    saveState();
                    isEditingBorder = true;
                }
                selectedShape->borderColor.r = borderCol[0];
                selectedShape->borderColor.g = borderCol[1];
                selectedShape->borderColor.b = borderCol[2];
            }


            //If it is not a line, allow the user to edit the fill color
            if (!dynamic_cast<Line*>(selectedShape)) {

                static bool isEditingFill = false;
                if (!ImGui::GetIO().MouseDown[0]) {
                    isEditingFill = false;
                }

                float fillCol[3] = { selectedShape->fillColor.r, selectedShape->fillColor.g, selectedShape->fillColor.b };

                if (ImGui::ColorEdit3("Fill Color", fillCol)) {
                    if (!isEditingFill) {
                        saveState();
                        isEditingFill = true;
                    }
                    selectedShape->fill = true; //If the user is changing the fill color, we can assume they want it filled
                    selectedShape->fillColor.r = fillCol[0];
                    selectedShape->fillColor.g = fillCol[1];
                    selectedShape->fillColor.b = fillCol[2];
                }
            }
            ImGui::Separator();
                ImGui::Text("Position: (%d, %d)", selectedShape->position.x, selectedShape->position.y);
                ImGui::Text("Bounding Box: Top-Left(%d, %d) - Bottom-Right(%d, %d)", selectedShape->boundingBox[0].x, selectedShape->boundingBox[0].y, selectedShape->boundingBox[1].x, selectedShape->boundingBox[1].y);
                if(ImGui::Checkbox("Edit Mode", &isEditing)) {
                    state = isEditing ? EDITING : SELECTING;
                }
            ImGui::Separator();
            if(ImGui::Button("Move Up")) {
                saveState();
                canvas.moveUp(selectedShape);
            };
            ImGui::SameLine();
            if (ImGui::Button("Move Down")) {
                saveState();
                canvas.moveDown(selectedShape);
            }
            if (ImGui::Button("Move Top")) {
                saveState();
                canvas.moveTop(selectedShape);
            }
            ImGui::SameLine();
            if (ImGui::Button("Move Bottom")) {
                saveState();
                canvas.moveBottom(selectedShape);
            }
            ImGui::Separator();
            if(ImGui::Button("Delete Shape")) {
                saveState();
                Shape* shapeToDelete = selectedShape;
                selectedShape = nullptr;
                canvas.deleteShape(shapeToDelete);
            }
            ImGui::End();
        }
    }
};

int main() {
    proyecto1 app;
    app.run();
    return 0;
}