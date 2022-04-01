add_rules("mode.debug", "mode.release")
set_languages("cxx17")

add_repositories("local xmake-repo")

add_requires("opengl", "sokol master")

rule("sokol_shaders")
	set_extensions(".glsl")

	before_buildcmd_file(function (target, batchcmds, shaderfile, opt)
		import("lib.detect.find_tool")

		local headerdir = path.join(target:autogendir(), "shaders")
		local name = path.basename(shaderfile)
		local ext = path.extension(shaderfile)
		local headerfile = path.join(headerdir, string.format("%s%s.h", name, ext))

		target:add('includedirs', target:autogendir())

		batchcmds:mkdir(headerdir)
		batchcmds:show_progress(opt.progress, "${color.build.object}generating headers for %s", shaderfile)
		batchcmds:vrunv("sokol-shdc", {"--input", shaderfile, "--output", headerfile, "--slang", "glsl330:hlsl5:metal_macos"})
		batchcmds:add_depfiles(shaderfile)
		batchcmds:set_depmtime(os.mtime(headerfile))
		batchcmds:set_depcache(target:dependfile(headerfile))
	end)
rule_end()

target("renderer_test")
	add_rules("sokol_shaders")
	set_kind("binary")
	add_files("src/*.cpp")
	add_files("src/shaders/*.glsl")
	add_includedirs("src")
	add_packages("opengl", "sokol", "sokol-tools-bin")