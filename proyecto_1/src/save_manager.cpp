#include "save_manager.h"

bool SaveManager::readColor(std::istream& stream, Color& color) {
    return static_cast<bool>(stream >> color.r >> color.g >> color.b);
}

bool SaveManager::readPoint(std::istream& stream, Point& point) {
    return static_cast<bool>(stream >> point.x >> point.y);
}

const char* SaveManager::shapeTypeToString(SavedShapeType type) {
    switch (type) {
        case SavedShapeType::Line: return "LINE";
        case SavedShapeType::Rectangle: return "RECTANGLE";
        case SavedShapeType::Triangle: return "TRIANGLE";
        case SavedShapeType::Ellipse: return "ELLIPSE";
        case SavedShapeType::Bezier: return "BEZIER";
        default: return "UNKNOWN";
    }
}

bool SaveManager::stringToShapeType(const std::string& token, SavedShapeType& type) {
    if (token == "LINE") { type = SavedShapeType::Line; return true; }
    if (token == "RECTANGLE") { type = SavedShapeType::Rectangle; return true; }
    if (token == "TRIANGLE") { type = SavedShapeType::Triangle; return true; }
    if (token == "ELLIPSE") { type = SavedShapeType::Ellipse; return true; }
    if (token == "BEZIER") { type = SavedShapeType::Bezier; return true; }
    return false;
}

bool SaveManager::savePainting(const std::string& filePath, const std::vector<Shape*>& shapes, const Color& backgroundColor) {
    std::ofstream output(filePath);
    if (!output.is_open()) return false;

    output << std::fixed << std::setprecision(6);
    output << "PAINTING_V1\n";
    output << "BACKGROUND " << backgroundColor.r << ' ' << backgroundColor.g << ' ' << backgroundColor.b << '\n';

    for (Shape* shape : shapes) {
        SavedShapeType type;
        std::vector<Point> pts;

        if (const auto* line = dynamic_cast<Line*>(shape)) {
            type = SavedShapeType::Line;
            pts = {line->getP0(), line->getP1()};
        } else if (const auto* rectangle = dynamic_cast<Rectangle*>(shape)) {
            type = SavedShapeType::Rectangle;
            pts = {rectangle->getP0(), rectangle->getP1()};
        } else if (const auto* triangle = dynamic_cast<Triangle*>(shape)) {
            type = SavedShapeType::Triangle;
            pts = {triangle->getP0(), triangle->getP1(), triangle->getP2()};
        } else if (const auto* ellipse = dynamic_cast<Ellipse*>(shape)) {
            type = SavedShapeType::Ellipse;
            pts = {ellipse->getP0(), ellipse->getP1()};
        } else if (const auto* bezier = dynamic_cast<Bezier*>(shape)) {
            type = SavedShapeType::Bezier;
            pts = bezier->getPoints();
        } else continue;

        // Write Format: SHAPE <TYPE> <FILL> <R G B border> <R G B fill> <NUM_POINTS> <X Y> <X Y> ...
        output << "SHAPE " << shapeTypeToString(type) << ' ' << static_cast<int>(shape->fill) << ' '
               << shape->borderColor.r << ' ' << shape->borderColor.g << ' ' << shape->borderColor.b << ' '
               << shape->fillColor.r << ' ' << shape->fillColor.g << ' ' << shape->fillColor.b << ' '
               << pts.size();

        for (const auto& p : pts) {
            output << ' ' << p.x << ' ' << p.y;
        }
        output << '\n';
    }

    std::cout << "Guardado en: " << filePath << std::endl;
    return true;
}

bool SaveManager::loadPainting(const std::string& filePath, std::vector<SavedShape>& outShapes, Color& outBackgroundColor) {
    std::ifstream input(filePath);
    if (!input.is_open()) return false;

    std::string header;
    if (!(input >> header) || header != "PAINTING_V1") return false;

    std::string token;
    while (input >> token) {
        if (token == "BACKGROUND") {
            if (!readColor(input, outBackgroundColor)) return false;
            continue;
        }

        if (token != "SHAPE") return false;

        std::string typeToken;
        int fillFlag = 0;
        if (!(input >> typeToken >> fillFlag)) return false;

        SavedShape record;
        if (!stringToShapeType(typeToken, record.type)) return false;
        record.fill = fillFlag != 0;

        if (!readColor(input, record.borderColor) || !readColor(input, record.fillColor)) return false;
        
        size_t numPoints;
        if (!(input >> numPoints)) return false;

        for (size_t i = 0; i < numPoints; ++i) {
            Point p;
            if (!readPoint(input, p)) return false;
            record.points.push_back(p);
        }

        outShapes.push_back(record);
    }

    std::cout << "Cargado desde: " << filePath << std::endl;
    return true;
}