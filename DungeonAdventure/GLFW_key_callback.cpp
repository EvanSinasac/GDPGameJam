#include "globalThings.h"
//Stuff done in class with Michael Feeney

#include <sstream>
#include <iostream>


// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    // From the help review session.
    // Move the camera to a location and look somewhere
    //if (key == GLFW_KEY_L && action == GLFW_PRESS)
    //{
    //    // Move the eye
    //    ::g_pFlyCamera->setEye(glm::vec3(8631.0f, -1487.0f, 13010.0f));

    //    // "look at" something.
    //    // Since we are using the fly camera, we can't just set the eye.
    //    //::g_pFlyCamera->setAt(glm::vec3(0.0f, 0.0f, 0.0f));

    //    ::g_pFlyCamera->Yaw_LeftRight(-80.0f);
    //    ::g_pFlyCamera->Pitch_UpDown(-10.0f);

    //}

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;


    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        for (std::vector<cMesh*>::iterator it = ::g_vec_pMeshes.begin();
            it != ::g_vec_pMeshes.end(); it++)
        {
            (*it)->bIsWireframe = !(*it)->bIsWireframe;
        }
        for (unsigned int index = 0; index != ::vec_pFSMEntities.size(); index++)
        {
            ::vec_pFSMEntities[index]->m_Mesh->bIsWireframe = !::vec_pFSMEntities[index]->m_Mesh->bIsWireframe;
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        ::g_bShowDebugObjects = !::g_bShowDebugObjects;
    }

    /*if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(1920, 1080, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }
    else if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(127, 64, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }
    else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(1280, 1024, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }
    else if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(3840, 2160, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }
    else if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(1200, 640, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }
    else if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
    {
        std::string errorString;
        if (!::g_pFBO->reset(1900, 1012, errorString))
        {
            std::cout << errorString << std::endl;
        }
    }*/
    
    
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        ::g_updateFBOResolution = !::g_updateFBOResolution;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        ::g_scene = !::g_scene;
    }

    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 0;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 1;
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 2;
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 3;
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 4;
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 5;
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 6;
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 7;
    }
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 8;
    }
    else if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        ::g_2DEffectOp = 9;
    }

    if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 1.0f;
    }
    else if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 2.0f;
    }
    else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
    {
        ::entitySpeedModifier = 3.0f;
    }

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }


    //   // If JUST the shift is down, move the "selected" object
       if ( bShiftDown && (!bControlDown) && (!bAltDown) )
       {
           
    
    
       }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

       // If JUST the ALT is down, move the "selected" light
    if ((!bShiftDown) && (!bControlDown) && bAltDown)
    {
        

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
