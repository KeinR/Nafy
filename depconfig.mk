bin/audio.AudioContext.o: src/audio/AudioContext.cpp src/audio/AudioContext.h src/audio/Device.h src/audio/oal.h
bin/audio.Device.o: src/audio/Device.cpp src/audio/Device.h src/audio/oal.h
bin/audio.SoundBuffer.o: src/audio/SoundBuffer.cpp src/audio/SoundBuffer.h src/audio/SoundData.h src/audio/oal.h
bin/audio.SoundData.o: src/audio/SoundData.cpp src/audio/SoundData.h src/audio/oal.h src/core/error.h
bin/audio.Speaker.o: src/audio/Speaker.cpp src/audio/SoundBuffer.h src/audio/SoundData.h src/audio/Speaker.h src/audio/oal.h
bin/core.context.o: src/core/context.cpp src/audio/AudioContext.h src/audio/Device.h src/audio/SoundBuffer.h src/audio/SoundData.h src/audio/Speaker.h src/audio/oal.h src/core/Context.h src/core/error.h src/core/glfw.h src/env/env.h src/gui/Image.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/render/Texture.h src/shaders/Shader.h src/shaders/ShaderFactory.h src/text/Font.h src/text/Text.h src/text/ftype.h
bin/core.error.o: src/core/error.cpp src/core/Context.h src/core/error.h src/core/glfw.h src/env/env.h
bin/core.TextLibrary.o: src/core/TextLibrary.cpp src/core/TextLibrary.h src/core/error.h src/text/Font.h src/text/FontFactory.h src/text/ftype.h
bin/core.Window.o: src/core/Window.cpp src/core/Context.h src/core/Window.h src/core/glfw.h src/env/env.h
bin/env.env.o: src/env/env.cpp src/audio/oal.h src/core/Context.h src/core/error.h src/core/glfw.h src/env/defs.h src/env/env.h
bin/game.BasicEvent.o: src/game/BasicEvent.cpp src/game/BasicEvent.h src/game/sceneEvent.h
bin/game.ButtonPrompt.o: src/game/ButtonPrompt.cpp src/core/Context.h src/core/callback.h src/core/glfw.h src/env/env.h src/game/ButtonPrompt.h src/game/sceneEvent.h src/gui/Button.h src/gui/Color.h src/gui/Node.h src/gui/Rectangle.h src/gui/TextRec.h src/gui/View.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/shaders/Shader.h src/text/Font.h src/text/Text.h src/text/TextCrawl.h src/text/ftype.h
bin/game.Scene.o: src/game/Scene.cpp src/core/Context.h src/core/error.h src/game/BasicEvent.h src/game/Scene.h src/game/TextString.h src/game/sceneEvent.h
bin/game.sceneEvent.o: src/game/sceneEvent.cpp src/core/Context.h src/game/BasicEvent.h src/game/Scene.h src/game/sceneEvent.h
bin/game.TextString.o: src/game/TextString.cpp src/core/Context.h src/core/glfw.h src/game/BasicEvent.h src/game/Scene.h src/game/TextString.h src/game/sceneEvent.h
bin/gui.Button.o: src/gui/Button.cpp src/core/Context.h src/core/callback.h src/core/glfw.h src/env/env.h src/gui/Button.h src/gui/Color.h src/gui/Rectangle.h src/gui/TextRec.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/shaders/Shader.h src/text/Font.h src/text/Text.h src/text/TextCrawl.h src/text/ftype.h
bin/gui.Color.o: src/gui/Color.cpp src/gui/Color.h
bin/gui.Image.o: src/gui/Image.cpp src/core/Context.h src/core/glfw.h src/env/defs.h src/env/env.h src/gui/Image.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/render/Texture.h src/shaders/Shader.h
bin/gui.Node.o: src/gui/Node.cpp src/gui/Node.h src/gui/renderable.h
bin/gui.Rectangle.o: src/gui/Rectangle.cpp src/core/Context.h src/core/glfw.h src/env/defs.h src/env/env.h src/gui/Color.h src/gui/Rectangle.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/shaders/Shader.h
bin/gui.TextRec.o: src/gui/TextRec.cpp src/gui/Color.h src/gui/Rectangle.h src/gui/TextRec.h src/gui/renderable.h src/render/Buffer.h src/render/Model.h src/shaders/Shader.h src/text/Font.h src/text/Text.h src/text/TextCrawl.h src/text/ftype.h
bin/gui.View.o: src/gui/View.cpp src/gui/Node.h src/gui/View.h src/gui/renderable.h
bin/render.Buffer.o: src/render/Buffer.cpp src/core/Context.h src/core/glfw.h src/env/env.h src/gui/renderable.h src/render/Buffer.h
bin/render.Model.o: src/render/Model.cpp src/core/Context.h src/core/glfw.h src/env/env.h src/render/Model.h src/shaders/Shader.h
bin/render.Texture.o: src/render/Texture.cpp src/core/glfw.h src/render/Texture.h
bin/shaders.Shader.o: src/shaders/Shader.cpp src/core/error.h src/core/glfw.h src/shaders/Shader.h
bin/shaders.ShaderFactory.o: src/shaders/ShaderFactory.cpp src/core/Context.h src/core/error.h src/core/glfw.h src/env/env.h src/shaders/Shader.h src/shaders/ShaderFactory.h
bin/text.Font.o: src/text/Font.cpp src/text/Font.h src/text/ftype.h
bin/text.FontFactory.o: src/text/FontFactory.cpp src/core/Context.h src/core/glfw.h src/env/env.h src/text/Font.h src/text/FontFactory.h src/text/ftype.h src/text/textdefs.h
bin/text.Text.o: src/text/Text.cpp src/core/Context.h src/core/glfw.h src/env/env.h src/gui/renderable.h src/shaders/Shader.h src/text/Font.h src/text/Text.h src/text/ftype.h src/text/textdefs.h
bin/text.TextCrawl.o: src/text/TextCrawl.cpp src/core/Context.h src/core/glfw.h src/env/env.h src/gui/renderable.h src/shaders/Shader.h src/text/Font.h src/text/Text.h src/text/TextCrawl.h src/text/ftype.h src/text/textdefs.h
bin/extern.glad.glad.o: extern/glad/glad.c
bin/extern.stb.stb_image.o: extern/stb/stb_image.c extern/stb/stb_image.h
bin/extern.stb.stb_image_write.o: extern/stb/stb_image_write.c extern/stb/stb_image_write.h
bin/extern.stb.stb_vorbis.o: extern/stb/stb_vorbis.c
