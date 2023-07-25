1. Project structure
    engine:
        audio
        common
        gfx
        main
        network
        system
        window


    gfx
        geometry
            figures
                v(Figure)
                Cross              
                Hexagon            
                Polygon             
                Rectangle           
                StrechedHexagon   
            transform
        math
        opengl
            gl_wrapper
                window
        stb
    engine
        control ?
        hud ?
        mechanics ?
        menu ?
        objects ?
        ui ?
        world ?
    common
        event ?
        logger
        utils
    main


2. Dependencies
        'gfx' dir branch should be exist without other directories (especially logger)
        'common' dir branch should be exist without other directories

3. Naming
    he::audio
    he::common
    he::gfx
    he::main
    he::network
    he::system
    he::window