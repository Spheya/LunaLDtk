#include <iostream>
#include <luna.hpp>
#include <lunaLDtk.hpp>

int main() {
    luna::setMessageCallback([](const char* message, const char* prefix, luna::MessageSeverity severity) { std::cout << '<' << prefix << "> " << message << std::endl; });

    luna::initialize();

    luna::Window window("Luna LDtk Example", 1440, 960);
    luna::ForwardRenderer renderer;

    luna::Camera camera(&window);
    camera.setProjectionType(luna::ProjectionType::Orthographic);
    camera.setBackgroundColor(luna::Color::Gray);
    camera.getTransform().position = glm::vec3(15.0f, -10.0f, 10.0f);
    camera.setOrthographicSize(20.0f);

    ldtk::Project ldtkProject;
    ldtkProject.loadFromFile("assets/Example.ldtk");
    auto tilesets = luna::loadTilesets(ldtkProject);
    luna::TileLevel level(ldtkProject.getWorld().getLevel("Level_0"), tilesets, 24.0f);

    while (!luna::isCloseRequested() && !window.isCloseRequested()) {
        luna::update();
        camera.updateAspect();
        renderer.beginFrame();

        if(luna::Input::isMouseButtonDown(luna::MouseButton::Right))
            luna::updateDebugCamera(camera);

        for (int i = 0; i < level.tileLayers().size(); i++) {
            const luna::TileLayer& layer = level.tileLayers()[i];

            luna::Transform transform(glm::vec3(0.0f, 0.0f, -i));
            renderer.push(&layer.getMesh(), transform.matrix(), &layer.getMaterial());
        }

        renderer.endFrame();
        renderer.render(camera);
        window.update();
    }

    luna::terminate();
}