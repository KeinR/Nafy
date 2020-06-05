EXT_OBJ = bin/extern.glad.glad.o bin/extern.stb.stb_image.o bin/extern.stb.stb_image_write.o bin/extern.stb.stb_truetype.o bin/extern.stb.stb_vorbis.o 
OBJ = bin/core.context.o bin/core.error.o bin/core.TextLibrary.o bin/core.Window.o bin/env.env.o bin/game.story.o bin/gui.Button.o bin/gui.Color.o bin/gui.Node.o bin/gui.Rectangle.o bin/gui.TextRec.o bin/gui.View.o bin/shaders.Shader.o bin/shaders.ShaderFactory.o bin/text.Font.o bin/text.FontFactory.o bin/text.Text.o bin/text.TextCrawl.o 
objects := $(OBJ) $(EXT_OBJ)
