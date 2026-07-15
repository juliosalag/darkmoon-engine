#include "darkmoon.hpp"


DarkMoon::Node* createScene3D(DarkMoon::DarkMoonEngine& engine) {
    // Node Scene 3D
    auto p_node3D = engine.CreateNode("Scene 3D", engine.GetRootNode());

    //Node: Rejilla
    engine.CreateGrid(10, 10.0f, D_GRAY, "Rejilla principal", p_node3D);

    // Node: Linea diagonal
    engine.CreatePoint3D({ -1.0f, 0.0f, 1.0f }, 5.0f, D_BLACK, "Punto principio linea", p_node3D);
    engine.CreateLine3D({ -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, -1.0f }, 2.0f, D_YELLOW_DARK, "Linea amarilla", p_node3D);
    engine.CreatePoint3D({ 1.0f, 1.0f, -1.0f }, 5.0f, D_BLACK, "Punto fin linea", p_node3D);

    // Node: Plano
    engine.CreatePlane({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, D_PINK, "Plano rosita", p_node3D);

    //Node: Cubo
    engine.CreateCube({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_AQUA_DARK, "Cubo azulito", p_node3D);
    engine.CreateCubeWires({ -30.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);
    //Node: Wireframe
    engine.CreateCubeWires({ 20.0f, 0.0f, -30.0f }, { 10.0f, 10.0f, 10.0f }, D_BLACK, "Wireframe", p_node3D);

    // Node: Modelo Dummy 3
     // auto modelDummy3 = engine.CreateModel("assets/Cofre/Texturas/Cofre.fbx", D_WHITE, "Modelo: Dummy 3", p_node3D);
     // modelDummy3->translate({ 30.0f, 0.0f, 0.0f });
     // modelDummy3->rotate({ 1.0f, 0.0f, 0.0f }, -90.0f);
     // auto eModel = dynamic_cast<DarkMoon::Model*>(modelDummy3->getEntity());
     // auto& m_meshes = eModel->getMeshes();

    // auto& am = AnimationManager::getInstance();
    // auto* animation = am.createAnimation("assets/Cofre/Texturas/Cofre.fbx", eModel->getboneInfoMap());

    // auto id = am.PlayAnimation(animation);
    // for (auto& mesh : m_meshes) {
    //     mesh->animID = id;
    // }

    // auto modelCharacter = engine.CreateModel("assets/MainCharacter/Main_character.fbx", D_WHITE, "Modelo MAin character", p_node3D);
    // modelCharacter->translate({ 0.0f, 0.0f, 30.0f });
    // modelCharacter->rotate({ 1.0f, 0.0f, 0.0f }, -90.0f);
    // // modelCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 90.0f);
    // auto eModel1 = dynamic_cast<DarkMoon::Model*>(modelCharacter->getEntity());
    // auto& m_meshes1 = eModel1->getMeshes();

    // auto* animation1 = am.createAnimation("assets/MainCharacter/Main_character.fbx", eModel1->getboneInfoMap());

    // auto id1 = am.PlayAnimation(animation1);
    // for (auto& mesh : m_meshes1) {
    //     mesh->animID = id1;
    // }

    return p_node3D;
}

DarkMoon::Node* createMainCharacter(DarkMoon::DarkMoonEngine& engine) {
    //Node: Modelo
    auto model = engine.CreateModel("assets/MainCharacter/Main_character.fbx", D_WHITE, "Modelo: Main Character", engine.GetRootNode());
    model->translate({ 0.0f, 0.0f, 0.0f });
    model->setScale({ 3.f, 3.f, 3.f });
    model->setRotation({ 1.0f, 0.0f, 0.0f }, -90.0f);
    model->rotate({0.0f, 1.0f, 0.0f}, 90.0f);
    auto eModel = dynamic_cast<DarkMoon::Model*>(model->getEntity());
    eModel->drawModel = true;

    return model;
}

void inputManager(DarkMoon::DarkMoonEngine& engine, DarkMoon::Node* nodeCharacter) {
    auto camera = engine.GetCamera();
    float velocity = 0.5f;

    if (engine.IsKeyDown(D_KEY_A)) { // +X -Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        nodeCharacter->translate({ velocity, 0.0f, -velocity });
        nodeCharacter->setRotation({ 1.0f, 0.0f, 0.0f }, -90.0f);
        nodeCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 0.0f);
        // nodeCharacter->setRotation({ 1.0f, 0.0f, 0.0f }, 45.0f);
    }
    if (engine.IsKeyDown(D_KEY_D)) { // -X +Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        nodeCharacter->translate({ -velocity, 0.0f, velocity });
        nodeCharacter->setRotation({ 1.0f, 0.0f, 0.0f }, -90.0f);
        nodeCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 180.0f);
    }
    if (engine.IsKeyDown(D_KEY_W)) { // +X +Z
        camera->position.x += velocity;
        camera->target.x += velocity;

        camera->position.z += velocity;
        camera->target.z += velocity;

        nodeCharacter->translate({ velocity, 0.0f, velocity });
        nodeCharacter->setRotation({ 1.0f, 0.0f, 0.0f }, -90.0f);
        nodeCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 270.0f);
    }
    if (engine.IsKeyDown(D_KEY_S)) { // -X -Z
        camera->position.x -= velocity;
        camera->target.x -= velocity;

        camera->position.z -= velocity;
        camera->target.z -= velocity;

        nodeCharacter->translate({ -velocity, 0.0f, -velocity });
        nodeCharacter->setRotation({ 1.0f, 0.0f, 0.0f }, -90.0f);
        nodeCharacter->rotate({ 0.0f, 1.0f, 0.0f }, 90.0f);
    }
    if (engine.IsKeyDown(D_KEY_G)) {
        nodeCharacter->scale({ 0.9,0.9,0.9 });
    }
    if (engine.IsKeyDown(D_KEY_J)) {
        nodeCharacter->scale({ 1.1,1.1,1.1 });
    }
    if (engine.IsKeyPressed(D_KEY_F7))
        engine.GetRootNode()->drawTree();

    nodeCharacter->translate({ 0.0f, 0.0f, 0.0f });
}

int main() {
    DarkMoon::DarkMoonEngine engine;

    if (engine.InitWindow(800, 600, "DarkMoon Engine")) {

        std::cout << "┌────────────────┐" << std::endl;
        std::cout << "│ Load Resources │" << std::endl;
        std::cout << "└────────────────┘" << std::endl;

        createScene3D(engine);
        auto mainCharacter = createMainCharacter(engine);
        auto modelcharacter = dynamic_cast<DarkMoon::Model*>(mainCharacter->getEntity());
        auto& m_meshes = modelcharacter->getMeshes();


        auto& boneInfoMap = modelcharacter->getboneInfoMap();
        auto& am = AnimationManager::getInstance();
        // auto& boneCount = modelcharacter->getBoneCount();
        // int count = modelcharacter->getBoneCount();
        std::vector<Animation*> anims = am.createAnimations("assets/MainCharacter/Main_character.fbx", boneInfoMap);

        auto bastonNode = engine.CreateModel("assets/MainCharacter/Baston_malo.obj", D_WHITE, "Modelo: Baston", mainCharacter);
        //bastonNode->translate({-2.62f, -0.65f, 3.415f});
        bastonNode->rotate({1.0f, 0.0f, 0.0f}, 90.0f);
        //bastonNode->scale({2.0f, 2.0f, 2.0f});
        


        // BoneInfo handBone;
        // // find character hand bone and set it to bastonmodel
        // for(int i = 0; i < boneInfoMap.size(); i++){
        //     if (boneInfoMap[i].name == "R.Antebrazo"){
        //         handBone = boneInfoMap[i];
        //     }
        // }
        auto bastonModel = dynamic_cast<DarkMoon::Model*>(bastonNode->getEntity());
        // bastonModel->setJoint(&handBone);

        //bastonModel->setTranslation({ 0.0f, 05.0f, 05.0f });
        //auto ebastonModel = dynamic_cast<DarkMoon::Model*>(bastonModel->getEntity());
        //ebastonModel->drawModel = true;

        // auto& characterMeshes = modelcharacter->getAdjuntMeshes();
        // for (auto& mesh : ebastonModel->getMeshes())
        // {
        //     characterMeshes.push_back(mesh);
        // }

        auto camera = engine.GetCamera();
        camera->position = { -0.0f, 40.0f, -60.0f };
        camera->fovy = 60.0f;

        engine.SetExitKey(D_KEY_F8);
        engine.SetTargetFPS(60);

        // std::cout << "┌──────┐" << std::endl;
        // std::cout << "│ Tree │" << std::endl;
        // std::cout << "└──────┘" << std::endl;

        auto id = am.PlayAnimation(anims[1]);
        for (auto& mesh : m_meshes) {
            if (mesh->hasBones)
                mesh->animID = id;
        }

        glm::vec3 position = { 3.59f, 3.3f, 1.f };
        glm::vec3 rot = { 0.0f, 0.0f, 1.0f };
        float angle = 198.f;
        while (!engine.WindowShouldClose()) {

            float deltatime = static_cast<float>(engine.GetFrameTime());

            // Logic

            inputManager(engine, mainCharacter);
            if (engine.IsKeyDown(D_KEY_X)) {
                am.StopAnimation(id);
                for (auto& mesh : m_meshes) {
                    mesh->animID = std::numeric_limits<std::size_t>::max();
                    for (auto& ver : mesh->vertices) {
                        for (int i = 0; i < 4; i++)
                        {
                            ver.boneIDs[i] = -1;
                            ver.weights[i] = 0.0f;
                        }
                    }
                }
            }
            if (engine.IsKeyDown(D_KEY_C)) {
                auto idxd = am.PlayAnimation(anims[10]);
                for (auto& mesh : m_meshes) {
                    mesh->animID = idxd;
                }
            }

            if (engine.IsKeyDown(D_KEY_4)) {
                position = { position.x + 0.1f, position.y, position.z };
            }
            else if (engine.IsKeyDown(D_KEY_R))
            {
                position = { position.x - 0.1f, position.y, position.z };
            }
            else if (engine.IsKeyDown(D_KEY_5))
            {
                position = { position.x, position.y + 0.1f, position.z };
            }
            else if (engine.IsKeyDown(D_KEY_T))
            {
                position = { position.x, position.y - 0.1f, position.z };
            }
            else if (engine.IsKeyDown(D_KEY_6))
            {
                position = { position.x, position.y, position.z + 0.1f };
            }
            else if (engine.IsKeyDown(D_KEY_Y))
            {
                position = { position.x, position.y, position.z - 0.1f };
            }
            else if (engine.IsKeyDown(D_KEY_7))
            {
                angle += 1.0f;
            }
            else if (engine.IsKeyDown(D_KEY_U))
            {
                angle -= 1.0f;
            }
            else if (engine.IsKeyDown(D_KEY_8))
            {
                rot = { 1.0f, 0.0f, 0.0f };
            }
            else if (engine.IsKeyDown(D_KEY_I))
            {
                rot = { 0.0f, 1.0f, 0.0f };
            }
            else if (engine.IsKeyDown(D_KEY_9))
            {
                rot = { 0.0f, 0.0f, 1.0f };
            }

            am.UpdateAnimation(deltatime);

            //get bone local transform from model
            auto bastoninfomap = bastonModel->getboneInfoMap();
            auto finalBoneMatrix = am.GetFinalBoneMatrices(0);

            glm::mat4 boneTransform = boneTransform = finalBoneMatrix[15];

            //Get bone parent transform
            glm::mat4 bastonModelatrix = bastonNode->getTransformationMatrix();
            //Apply transform to model
            //bastonNode->setTransformationMatrix(bastonModelatrix * boneTransform);

            // for(int i = 0; i <  4; i++){
            //     for(int j = 0; j < 4; j++){
            //         boneTransform[j][i] *= 2000;
            //     }
            // }

            glm::mat4 bastonInitMatrix = glm::mat4(1.0f);
            glm::vec3 initTranslation = {-2.6283f, 3.4105f,  -0.6563f};
            initTranslation = {+0.6563f, -2.6283f,  3.4105f};
            bastonInitMatrix = glm::translate(bastonInitMatrix, initTranslation);
            
            glm::mat4 newbastonmatrix = boneTransform * bastonInitMatrix;

            bastonNode->setTransformationMatrix(newbastonmatrix);


            // for(int i = 0; i <  4; i++){
            //     for(int j = 0; j < 4; j++){
            //         std::cout << boneTransform[j][i] << "   ";
            //     }
            //     std::cout << std::endl;
            // }
            // std::cout << std::endl;

            // for(int i = 0; i <  4; i++){
            //     for(int j = 0; j < 4; j++){
            //         std::cout << newbastonmatrix[j][i] << "   ";
            //     }
            //     std::cout << std::endl;
            // }
            // std::cout << std::endl;
            //bastonNode->tras({0.1f, 0.0f, 0.0f});

            // Obtiene la matriz de transformación del hueso
            // glm::mat4 boneTransform = am.getBoneTransform("R.Antebrazo", anims[10]);

            // // Obtiene la matriz de transformación del personaje principal
            // glm::mat4 characterTransform = mainCharacter->getTransformationMatrix();

            // // Crea una matriz de transformación para el reposicionamiento y la rotación
            // glm::mat4 offsetTransform = glm::translate(glm::mat4(1.0f), position); // Mueve el modelo hijo 1 unidad hacia arriba
            // offsetTransform = glm::rotate(offsetTransform, glm::radians(angle), rot); // Rota el modelo hijo 45 grados alrededor del eje Y
            // // offsetTransform = glm::rotate(offsetTransform, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, .0f)); // Rota el modelo hijo 45 grados alrededor del eje Y

            // // Combina las matrices de transformación
            // glm::mat4 combinedTransform = characterTransform * boneTransform;

            // Aplica la transformación combinada al modelo de la varita
            //bastonModel->setTransformationMatrix(combinedTransform);
            // for (int i = 0; i < 4; i++)
            // {
            //     for (int j = 0; j < 4; j++)
            //     {
            //         std::cout << transMatrix[i][j] << " ";
            //     }
            //     std::cout << std::endl;
            // }
            // std::cout << std::endl;

            engine.BeginDrawing();

            engine.ClearBackground(D_WHITE);
            engine.GetRootNode()->traverse(glm::mat4());

            engine.EndDrawing();
        }

        engine.CloseWindow();
    }

    return 0;
}
/*
    Patron Dirty //
*/