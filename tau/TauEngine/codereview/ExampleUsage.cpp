    void exampleUsage(HDC hdc, HGLRC renderingContex, GLuint shaderProgram)
    {
        RenderingPipeline rp(hdc, renderingContex);

        while(true)
        {
            rp.pushInstruction(RenderingOpcode::ACTIVATE_SHADER_PROGRAM, ParameterPack(shaderProgram));
            // ...
            rp.pushInstruction(RenderingOpcode::FINISH_RENDER, ParameterPack());
        }
    }
