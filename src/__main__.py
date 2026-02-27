from setup_ursina_android import setup_ursina_android
setup_ursina_android()
from ursina import *
from browser import window as htmlwindow

# Initialize ursina app
app = Ursina(editor_ui_enabled=False)

# Window settings
window.exit_button.enabled = False
# window.size = Vec2(480, 1080)
window.color = color.hex("#851be2")

# Widget
Text("Ursina for\nMobile", scale=2, position=(.1, .4), origin=(0, 0))
Entity(model="quad.ursinamesh", scale=.1, position=(-.1, .4), origin=(0, 0), texture="ursina", parent=camera.ui)
Text("Default app", position=(.0, .32), origin=(0, 0))

cube = Entity(model="cube.ursinamesh", scale=.1, position=(0, 0), origin=(0, 0), color=color.red, parent=camera.ui)
main_button = Button("Change background color", color=color.lime, pressed_color=color.yellow, origin=(0, 0), position=(0, -.3), scale=(.4, .07), text_color=color.gray) # type: ignore

# Change window color to random color on button click
def changeBackground():
    window.color = color.random_color()

# Make cube rotating
def update():
    cube.rotation += Vec3(1, 1, 0)

def update_screen_resolution(): 
    width = htmlwindow.innerWidth
    height = htmlwindow.innerHeight

    window.size = Vec2(width, height)
    window.update_aspect_ratio()

# Add the onclick event
main_button.on_click = changeBackground

htmlwindow.addEventListener("resize", update_screen_resolution)
update_screen_resolution()

# Run your app
# app.run()