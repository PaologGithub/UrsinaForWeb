import os
import sys
from direct.stdpy.file import open, exists
from panda3d.core import ConfigVariableString

def setup_ursina_android():
    finish()

""" def setup_assets():
    import zlib
    import json

    if exists(os.path.join(os.getcwd(), "assets.gen")):
        return

    with open("/android_asset/assets/assets.gen", "rb") as file:
        decompressed = zlib.decompress(file.read()) # type: ignore
        data = json.loads(decompressed)
        open(os.path.join(os.getcwd(), "assets.gen"), "wb").write(file.read()) # type: ignore
    
    os.mkdir(ursina_assets)
    os.mkdir("game_assets")

    for root_folder in data:
        for file in data[root_folder]:
            file_data = data[root_folder][file]
            dir = file_data["dir"]

            src_path = f"/android_asset/assets/{dir}"
            dest_path = os.path.join(os.getcwd(), dir)

            os.makedirs(os.path.dirname(dir), exist_ok=True)

            with open(src_path, 'rb') as src_file, open(dest_path, 'wb') as dest_file:
                dest_file.write(src_file.read()) # type: ignore
                print("Writed file: " + dest_path) """

def finish():
    from ursina import application
    from pathlib import Path

    ursina_assets = Path("assets") / "ursina_assets"
    game_assets = Path("assets") / "game_assets"

    application.package_folder = ursina_assets
    application.asset_folder = game_assets

    application.scenes_folder = game_assets / 'scenes/'
    application.scripts_folder = game_assets / 'scripts/'
    application.fonts_folder = game_assets / 'fonts/'

    # Reset the sub paths
    application.internal_models_folder = ursina_assets / 'models/'
    application.internal_models_compressed_folder = ursina_assets / 'models_compressed/'
    application.internal_scripts_folder = ursina_assets / 'scripts/'
    application.internal_textures_folder = ursina_assets / 'textures/'
    application.internal_fonts_folder = ursina_assets / 'fonts/'
    application.internal_audio_folder = ursina_assets / 'audio/'