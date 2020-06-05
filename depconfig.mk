bin/core.context.o: src/core/context.cpp src/core/context.h src/core/error.h src/env/env.h src/shaders/ShaderFactory.h src/text/Font.h src/text/Text.h src/text/ftype.h
bin/core.error.o: src/core/error.cpp src/core/error.h src/env/env.h
bin/core.TextLibrary.o: src/core/TextLibrary.cpp src/core/TextLibrary.h src/core/error.h
bin/core.Window.o: src/core/Window.cpp src/core/Window.h src/env/env.h
bin/env.env.o: src/env/env.cpp src/core/error.h src/env/defs.h src/env/env.h
bin/game.story.o: src/game/story.cpp src/core/context.h src/core/error.h src/core/glfw.h src/game/story.h
bin/gui.Button.o: src/gui/Button.cpp src/core/glfw.h src/env/env.h src/gui/Button.h
bin/gui.Color.o: src/gui/Color.cpp src/gui/Color.h
bin/gui.Node.o: src/gui/Node.cpp src/gui/Node.h
bin/gui.Rectangle.o: src/gui/Rectangle.cpp src/core/glfw.h src/env/defs.h src/env/env.h src/gui/Rectangle.h
bin/gui.TextRec.o: src/gui/TextRec.cpp src/gui/TextRec.h
bin/gui.View.o: src/gui/View.cpp src/gui/View.h
bin/shaders.Shader.o: src/shaders/Shader.cpp src/core/glfw.h src/shaders/Shader.h
bin/shaders.ShaderFactory.o: src/shaders/ShaderFactory.cpp src/core/error.h src/core/glfw.h src/env/env.h src/shaders/ShaderFactory.h
bin/text.Font.o: src/text/Font.cpp src/text/Font.h
bin/text.FontFactory.o: src/text/FontFactory.cpp src/text/FontFactory.h src/text/textdefs.h
bin/text.Text.o: src/text/Text.cpp src/text/Text.h src/text/textdefs.h
bin/text.TextCrawl.o: src/text/TextCrawl.cpp src/text/TextCrawl.h src/text/textdefs.h
bin/extern.glad.glad.o: extern/glad/glad.c
bin/extern.stb.stb_image.o: extern/stb/stb_image.c extern/stb/stb_image.h
bin/extern.stb.stb_image_write.o: extern/stb/stb_image_write.c extern/stb/stb_image_write.h
bin/extern.stb.stb_truetype.o: extern/stb/stb_truetype.c extern/stb/stb_truetype.h
bin/extern.stb.stb_vorbis.o: extern/stb/stb_vorbis.c
