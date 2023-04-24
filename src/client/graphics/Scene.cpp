/**************************************************
Scene.cpp contains the implementation of the draw command
Renders all objects with DFS tree traversal
adapted from CSE 167 - Matthew
*****************************************************/
#include "Scene.h"

// The scene init definition 
#include "Scene.inl"


using namespace glm;

void Scene::update(float deltaTime) {
    //if (player) player->update(deltaTime);

    for (auto e : gamethings) {
        e->update(deltaTime);
    }
}

void Scene::draw(const glm::mat4& viewProjMtx) {
    // Pre-draw sequence:
    //camera->computeMatrices();

    // Define stacks for depth-first search (DFS)
    std::stack < Node* > dfs_stack;
    std::stack < mat4 >  matrix_stack;

    // Initialize the current state variable for DFS
    Node* cur = node["world"]; // root of the tree
    //mat4 cur_VM = camera->view;   // 167 implementation uses VM, we can use model matrix
    mat4 cur_MMtx = cur->transformMtx;

    dfs_stack.push(cur);
    matrix_stack.push(cur_MMtx);

    while (!dfs_stack.empty()) {
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the size of the graph.
        // Note that, at any time, the stack does not contain repeated element.
        if (dfs_stack.size() > node.size()) {
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        // top-pop the stacks
        cur = dfs_stack.top();
        dfs_stack.pop();
        cur_MMtx = matrix_stack.top();
        matrix_stack.pop();

        // draw the model of our current node
        if(cur->model)
            cur->model->draw(viewProjMtx, cur_MMtx);

        // Continue the DFS: put all the child nodes of the current node in the stack
        for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
            dfs_stack.push(cur->childnodes[i]);
            matrix_stack.push(cur_MMtx * (cur->childnodes[i]->transformMtx));
        }

    } // End of DFS while loop.

}


#include "imgui/imgui.h"
void treeChildren(ImGuiTreeNodeFlags node_flags, bool isOpen, Node* par)
{
    if (isOpen)
    {
        for (auto child : par->childnodes)
        {
            if (child->childnodes.empty())
            {
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                //ImGui::TreeNodeEx((void*)(intptr_t)par, node_flags, child.name.c_str());
            }
            else
            {
                //bool o = ImGui::TreeNodeEx((void*)(intptr_t)par, node_flags, child.name.c_str());
                ImGui::TreePop();
                //treeChildren(node_flags, o, p);
            }
        }
        ImGui::TreePop();
    }
}

void Scene::gui() {
    ImGui::Begin("Scene Info");

    //show a list of game objects
    //ImGui::LabelText("", "gfx nodes");

    if (ImGui::TreeNode("--- node[] tree ---"))
    {
        std::stack < Node* > dfs_stack_gui;
        Node* cur = node["world"]; // root of the tree
        dfs_stack_gui.push(cur);

        int gui_id = 0;
        while (!dfs_stack_gui.empty()) {
            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);

            // top-pop the stacks
            cur = dfs_stack_gui.top();
            dfs_stack_gui.pop();

            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (cur->childnodes.empty()) node_flags |= ImGuiTreeNodeFlags_Leaf;
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)gui_id, node_flags, cur->name.c_str());

            if (node_open) {
                for (unsigned int i = 0; i < cur->childnodes.size(); i++) {
                    dfs_stack_gui.push(cur->childnodes[i]);
                }
                ImGui::TreePop();
            }
            gui_id++;

            ImGui::PopStyleVar();

        } // End of DFS while loop

        ImGui::TreePop();
    }

    ImGui::End();
}