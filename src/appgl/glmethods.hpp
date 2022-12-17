#pragma once

#include "opengl.hpp"

struct GLMethods
{
  PFNGLCULLFACEPROC CullFace;
  PFNGLFRONTFACEPROC FrontFace;
  PFNGLHINTPROC Hint;
  PFNGLLINEWIDTHPROC LineWidth;
  PFNGLPOINTSIZEPROC PointSize;
  PFNGLPOLYGONMODEPROC PolygonMode;
  PFNGLSCISSORPROC Scissor;
  PFNGLTEXPARAMETERFPROC TexParameterf;
  PFNGLTEXPARAMETERFVPROC TexParameterfv;
  PFNGLTEXPARAMETERIPROC TexParameteri;
  PFNGLTEXPARAMETERIVPROC TexParameteriv;
  PFNGLTEXIMAGE1DPROC TexImage1D;
  PFNGLTEXIMAGE2DPROC TexImage2D;
  PFNGLDRAWBUFFERPROC DrawBuffer;
  PFNGLCLEARPROC Clear;
  PFNGLCLEARCOLORPROC ClearColor;
  PFNGLCLEARSTENCILPROC ClearStencil;
  PFNGLCLEARDEPTHPROC ClearDepth;
  PFNGLSTENCILMASKPROC StencilMask;
  PFNGLCOLORMASKPROC ColorMask;
  PFNGLDEPTHMASKPROC DepthMask;
  PFNGLDISABLEPROC Disable;
  PFNGLENABLEPROC Enable;
  PFNGLFINISHPROC Finish;
  PFNGLFLUSHPROC Flush;
  PFNGLBLENDFUNCPROC BlendFunc;
  PFNGLLOGICOPPROC LogicOp;
  PFNGLSTENCILFUNCPROC StencilFunc;
  PFNGLSTENCILOPPROC StencilOp;
  PFNGLDEPTHFUNCPROC DepthFunc;
  PFNGLPIXELSTOREFPROC PixelStoref;
  PFNGLPIXELSTOREIPROC PixelStorei;
  PFNGLREADBUFFERPROC ReadBuffer;
  PFNGLREADPIXELSPROC ReadPixels;
  PFNGLGETBOOLEANVPROC GetBooleanv;
  PFNGLGETDOUBLEVPROC GetDoublev;
  PFNGLGETERRORPROC GetError;
  PFNGLGETFLOATVPROC GetFloatv;
  PFNGLGETINTEGERVPROC GetIntegerv;
  PFNGLGETSTRINGPROC GetString;
  PFNGLGETTEXIMAGEPROC GetTexImage;
  PFNGLGETTEXPARAMETERFVPROC GetTexParameterfv;
  PFNGLGETTEXPARAMETERIVPROC GetTexParameteriv;
  PFNGLGETTEXLEVELPARAMETERFVPROC GetTexLevelParameterfv;
  PFNGLGETTEXLEVELPARAMETERIVPROC GetTexLevelParameteriv;
  PFNGLISENABLEDPROC IsEnabled;
  PFNGLDEPTHRANGEPROC DepthRange;
  PFNGLVIEWPORTPROC Viewport;
  PFNGLDRAWARRAYSPROC DrawArrays;
  PFNGLDRAWELEMENTSPROC DrawElements;
  PFNGLGETPOINTERVPROC GetPointerv;
  PFNGLPOLYGONOFFSETPROC PolygonOffset;
  PFNGLCOPYTEXIMAGE1DPROC CopyTexImage1D;
  PFNGLCOPYTEXIMAGE2DPROC CopyTexImage2D;
  PFNGLCOPYTEXSUBIMAGE1DPROC CopyTexSubImage1D;
  PFNGLCOPYTEXSUBIMAGE2DPROC CopyTexSubImage2D;
  PFNGLTEXSUBIMAGE1DPROC TexSubImage1D;
  PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
  PFNGLBINDTEXTUREPROC BindTexture;
  PFNGLDELETETEXTURESPROC DeleteTextures;
  PFNGLGENTEXTURESPROC GenTextures;
  PFNGLISTEXTUREPROC IsTexture;
  PFNGLDRAWRANGEELEMENTSPROC DrawRangeElements;
  PFNGLTEXIMAGE3DPROC TexImage3D;
  PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;
  PFNGLCOPYTEXSUBIMAGE3DPROC CopyTexSubImage3D;
  PFNGLACTIVETEXTUREPROC ActiveTexture;
  PFNGLSAMPLECOVERAGEPROC SampleCoverage;
  PFNGLCOMPRESSEDTEXIMAGE3DPROC CompressedTexImage3D;
  PFNGLCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
  PFNGLCOMPRESSEDTEXIMAGE1DPROC CompressedTexImage1D;
  PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC CompressedTexSubImage3D;
  PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC CompressedTexSubImage2D;
  PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC CompressedTexSubImage1D;
  PFNGLGETCOMPRESSEDTEXIMAGEPROC GetCompressedTexImage;
  PFNGLBLENDFUNCSEPARATEPROC BlendFuncSeparate;
  PFNGLMULTIDRAWARRAYSPROC MultiDrawArrays;
  PFNGLMULTIDRAWELEMENTSPROC MultiDrawElements;
  PFNGLPOINTPARAMETERFPROC PointParameterf;
  PFNGLPOINTPARAMETERFVPROC PointParameterfv;
  PFNGLPOINTPARAMETERIPROC PointParameteri;
  PFNGLPOINTPARAMETERIVPROC PointParameteriv;
  PFNGLBLENDCOLORPROC BlendColor;
  PFNGLBLENDEQUATIONPROC BlendEquation;
  PFNGLGENQUERIESPROC GenQueries;
  PFNGLDELETEQUERIESPROC DeleteQueries;
  PFNGLISQUERYPROC IsQuery;
  PFNGLBEGINQUERYPROC BeginQuery;
  PFNGLENDQUERYPROC EndQuery;
  PFNGLGETQUERYIVPROC GetQueryiv;
  PFNGLGETQUERYOBJECTIVPROC GetQueryObjectiv;
  PFNGLGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
  PFNGLBINDBUFFERPROC BindBuffer;
  PFNGLDELETEBUFFERSPROC DeleteBuffers;
  PFNGLGENBUFFERSPROC GenBuffers;
  PFNGLISBUFFERPROC IsBuffer;
  PFNGLBUFFERDATAPROC BufferData;
  PFNGLBUFFERSUBDATAPROC BufferSubData;
  PFNGLGETBUFFERSUBDATAPROC GetBufferSubData;
  PFNGLMAPBUFFERPROC MapBuffer;
  PFNGLUNMAPBUFFERPROC UnmapBuffer;
  PFNGLGETBUFFERPARAMETERIVPROC GetBufferParameteriv;
  PFNGLGETBUFFERPOINTERVPROC GetBufferPointerv;
  PFNGLBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
  PFNGLDRAWBUFFERSPROC DrawBuffers;
  PFNGLSTENCILOPSEPARATEPROC StencilOpSeparate;
  PFNGLSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
  PFNGLSTENCILMASKSEPARATEPROC StencilMaskSeparate;
  PFNGLATTACHSHADERPROC AttachShader;
  PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
  PFNGLCOMPILESHADERPROC CompileShader;
  PFNGLCREATEPROGRAMPROC CreateProgram;
  PFNGLCREATESHADERPROC CreateShader;
  PFNGLDELETEPROGRAMPROC DeleteProgram;
  PFNGLDELETESHADERPROC DeleteShader;
  PFNGLDETACHSHADERPROC DetachShader;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
  PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
  PFNGLGETACTIVEATTRIBPROC GetActiveAttrib;
  PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;
  PFNGLGETATTACHEDSHADERSPROC GetAttachedShaders;
  PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
  PFNGLGETPROGRAMIVPROC GetProgramiv;
  PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
  PFNGLGETSHADERIVPROC GetShaderiv;
  PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
  PFNGLGETSHADERSOURCEPROC GetShaderSource;
  PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
  PFNGLGETUNIFORMFVPROC GetUniformfv;
  PFNGLGETUNIFORMIVPROC GetUniformiv;
  PFNGLGETVERTEXATTRIBDVPROC GetVertexAttribdv;
  PFNGLGETVERTEXATTRIBFVPROC GetVertexAttribfv;
  PFNGLGETVERTEXATTRIBIVPROC GetVertexAttribiv;
  PFNGLGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerv;
  PFNGLISPROGRAMPROC IsProgram;
  PFNGLISSHADERPROC IsShader;
  PFNGLLINKPROGRAMPROC LinkProgram;
  PFNGLSHADERSOURCEPROC ShaderSource;
  PFNGLUSEPROGRAMPROC UseProgram;
  PFNGLUNIFORM1FPROC Uniform1f;
  PFNGLUNIFORM2FPROC Uniform2f;
  PFNGLUNIFORM3FPROC Uniform3f;
  PFNGLUNIFORM4FPROC Uniform4f;
  PFNGLUNIFORM1IPROC Uniform1i;
  PFNGLUNIFORM2IPROC Uniform2i;
  PFNGLUNIFORM3IPROC Uniform3i;
  PFNGLUNIFORM4IPROC Uniform4i;
  PFNGLUNIFORM1FVPROC Uniform1fv;
  PFNGLUNIFORM2FVPROC Uniform2fv;
  PFNGLUNIFORM3FVPROC Uniform3fv;
  PFNGLUNIFORM4FVPROC Uniform4fv;
  PFNGLUNIFORM1IVPROC Uniform1iv;
  PFNGLUNIFORM2IVPROC Uniform2iv;
  PFNGLUNIFORM3IVPROC Uniform3iv;
  PFNGLUNIFORM4IVPROC Uniform4iv;
  PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2fv;
  PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3fv;
  PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
  PFNGLVALIDATEPROGRAMPROC ValidateProgram;
  PFNGLVERTEXATTRIB1DPROC VertexAttrib1d;
  PFNGLVERTEXATTRIB1DVPROC VertexAttrib1dv;
  PFNGLVERTEXATTRIB1FPROC VertexAttrib1f;
  PFNGLVERTEXATTRIB1FVPROC VertexAttrib1fv;
  PFNGLVERTEXATTRIB1SPROC VertexAttrib1s;
  PFNGLVERTEXATTRIB1SVPROC VertexAttrib1sv;
  PFNGLVERTEXATTRIB2DPROC VertexAttrib2d;
  PFNGLVERTEXATTRIB2DVPROC VertexAttrib2dv;
  PFNGLVERTEXATTRIB2FPROC VertexAttrib2f;
  PFNGLVERTEXATTRIB2FVPROC VertexAttrib2fv;
  PFNGLVERTEXATTRIB2SPROC VertexAttrib2s;
  PFNGLVERTEXATTRIB2SVPROC VertexAttrib2sv;
  PFNGLVERTEXATTRIB3DPROC VertexAttrib3d;
  PFNGLVERTEXATTRIB3DVPROC VertexAttrib3dv;
  PFNGLVERTEXATTRIB3FPROC VertexAttrib3f;
  PFNGLVERTEXATTRIB3FVPROC VertexAttrib3fv;
  PFNGLVERTEXATTRIB3SPROC VertexAttrib3s;
  PFNGLVERTEXATTRIB3SVPROC VertexAttrib3sv;
  PFNGLVERTEXATTRIB4NBVPROC VertexAttrib4Nbv;
  PFNGLVERTEXATTRIB4NIVPROC VertexAttrib4Niv;
  PFNGLVERTEXATTRIB4NSVPROC VertexAttrib4Nsv;
  PFNGLVERTEXATTRIB4NUBPROC VertexAttrib4Nub;
  PFNGLVERTEXATTRIB4NUBVPROC VertexAttrib4Nubv;
  PFNGLVERTEXATTRIB4NUIVPROC VertexAttrib4Nuiv;
  PFNGLVERTEXATTRIB4NUSVPROC VertexAttrib4Nusv;
  PFNGLVERTEXATTRIB4BVPROC VertexAttrib4bv;
  PFNGLVERTEXATTRIB4DPROC VertexAttrib4d;
  PFNGLVERTEXATTRIB4DVPROC VertexAttrib4dv;
  PFNGLVERTEXATTRIB4FPROC VertexAttrib4f;
  PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
  PFNGLVERTEXATTRIB4IVPROC VertexAttrib4iv;
  PFNGLVERTEXATTRIB4SPROC VertexAttrib4s;
  PFNGLVERTEXATTRIB4SVPROC VertexAttrib4sv;
  PFNGLVERTEXATTRIB4UBVPROC VertexAttrib4ubv;
  PFNGLVERTEXATTRIB4UIVPROC VertexAttrib4uiv;
  PFNGLVERTEXATTRIB4USVPROC VertexAttrib4usv;
  PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
  PFNGLUNIFORMMATRIX2X3FVPROC UniformMatrix2x3fv;
  PFNGLUNIFORMMATRIX3X2FVPROC UniformMatrix3x2fv;
  PFNGLUNIFORMMATRIX2X4FVPROC UniformMatrix2x4fv;
  PFNGLUNIFORMMATRIX4X2FVPROC UniformMatrix4x2fv;
  PFNGLUNIFORMMATRIX3X4FVPROC UniformMatrix3x4fv;
  PFNGLUNIFORMMATRIX4X3FVPROC UniformMatrix4x3fv;
  PFNGLCOLORMASKIPROC ColorMaski;
  PFNGLGETBOOLEANI_VPROC GetBooleani_v;
  PFNGLGETINTEGERI_VPROC GetIntegeri_v;
  PFNGLENABLEIPROC Enablei;
  PFNGLDISABLEIPROC Disablei;
  PFNGLISENABLEDIPROC IsEnabledi;
  PFNGLBEGINTRANSFORMFEEDBACKPROC BeginTransformFeedback;
  PFNGLENDTRANSFORMFEEDBACKPROC EndTransformFeedback;
  PFNGLBINDBUFFERRANGEPROC BindBufferRange;
  PFNGLBINDBUFFERBASEPROC BindBufferBase;
  PFNGLTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
  PFNGLGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
  PFNGLCLAMPCOLORPROC ClampColor;
  PFNGLBEGINCONDITIONALRENDERPROC BeginConditionalRender;
  PFNGLENDCONDITIONALRENDERPROC EndConditionalRender;
  PFNGLVERTEXATTRIBIPOINTERPROC VertexAttribIPointer;
  PFNGLGETVERTEXATTRIBIIVPROC GetVertexAttribIiv;
  PFNGLGETVERTEXATTRIBIUIVPROC GetVertexAttribIuiv;
  PFNGLVERTEXATTRIBI1IPROC VertexAttribI1i;
  PFNGLVERTEXATTRIBI2IPROC VertexAttribI2i;
  PFNGLVERTEXATTRIBI3IPROC VertexAttribI3i;
  PFNGLVERTEXATTRIBI4IPROC VertexAttribI4i;
  PFNGLVERTEXATTRIBI1UIPROC VertexAttribI1ui;
  PFNGLVERTEXATTRIBI2UIPROC VertexAttribI2ui;
  PFNGLVERTEXATTRIBI3UIPROC VertexAttribI3ui;
  PFNGLVERTEXATTRIBI4UIPROC VertexAttribI4ui;
  PFNGLVERTEXATTRIBI1IVPROC VertexAttribI1iv;
  PFNGLVERTEXATTRIBI2IVPROC VertexAttribI2iv;
  PFNGLVERTEXATTRIBI3IVPROC VertexAttribI3iv;
  PFNGLVERTEXATTRIBI4IVPROC VertexAttribI4iv;
  PFNGLVERTEXATTRIBI1UIVPROC VertexAttribI1uiv;
  PFNGLVERTEXATTRIBI2UIVPROC VertexAttribI2uiv;
  PFNGLVERTEXATTRIBI3UIVPROC VertexAttribI3uiv;
  PFNGLVERTEXATTRIBI4UIVPROC VertexAttribI4uiv;
  PFNGLVERTEXATTRIBI4BVPROC VertexAttribI4bv;
  PFNGLVERTEXATTRIBI4SVPROC VertexAttribI4sv;
  PFNGLVERTEXATTRIBI4UBVPROC VertexAttribI4ubv;
  PFNGLVERTEXATTRIBI4USVPROC VertexAttribI4usv;
  PFNGLGETUNIFORMUIVPROC GetUniformuiv;
  PFNGLBINDFRAGDATALOCATIONPROC BindFragDataLocation;
  PFNGLGETFRAGDATALOCATIONPROC GetFragDataLocation;
  PFNGLUNIFORM1UIPROC Uniform1ui;
  PFNGLUNIFORM2UIPROC Uniform2ui;
  PFNGLUNIFORM3UIPROC Uniform3ui;
  PFNGLUNIFORM4UIPROC Uniform4ui;
  PFNGLUNIFORM1UIVPROC Uniform1uiv;
  PFNGLUNIFORM2UIVPROC Uniform2uiv;
  PFNGLUNIFORM3UIVPROC Uniform3uiv;
  PFNGLUNIFORM4UIVPROC Uniform4uiv;
  PFNGLTEXPARAMETERIIVPROC TexParameterIiv;
  PFNGLTEXPARAMETERIUIVPROC TexParameterIuiv;
  PFNGLGETTEXPARAMETERIIVPROC GetTexParameterIiv;
  PFNGLGETTEXPARAMETERIUIVPROC GetTexParameterIuiv;
  PFNGLCLEARBUFFERIVPROC ClearBufferiv;
  PFNGLCLEARBUFFERUIVPROC ClearBufferuiv;
  PFNGLCLEARBUFFERFVPROC ClearBufferfv;
  PFNGLCLEARBUFFERFIPROC ClearBufferfi;
  PFNGLGETSTRINGIPROC GetStringi;
  PFNGLISRENDERBUFFERPROC IsRenderbuffer;
  PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
  PFNGLDELETERENDERBUFFERSPROC DeleteRenderbuffers;
  PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
  PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
  PFNGLGETRENDERBUFFERPARAMETERIVPROC GetRenderbufferParameteriv;
  PFNGLISFRAMEBUFFERPROC IsFramebuffer;
  PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
  PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
  PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
  PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
  PFNGLFRAMEBUFFERTEXTURE1DPROC FramebufferTexture1D;
  PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
  PFNGLFRAMEBUFFERTEXTURE3DPROC FramebufferTexture3D;
  PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
  PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetFramebufferAttachmentParameteriv;
  PFNGLGENERATEMIPMAPPROC GenerateMipmap;
  PFNGLBLITFRAMEBUFFERPROC BlitFramebuffer;
  PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC RenderbufferStorageMultisample;
  PFNGLFRAMEBUFFERTEXTURELAYERPROC FramebufferTextureLayer;
  PFNGLMAPBUFFERRANGEPROC MapBufferRange;
  PFNGLFLUSHMAPPEDBUFFERRANGEPROC FlushMappedBufferRange;
  PFNGLBINDVERTEXARRAYPROC BindVertexArray;
  PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
  PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
  PFNGLISVERTEXARRAYPROC IsVertexArray;
  PFNGLDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
  PFNGLDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
  PFNGLTEXBUFFERPROC TexBuffer;
  PFNGLPRIMITIVERESTARTINDEXPROC PrimitiveRestartIndex;
  PFNGLCOPYBUFFERSUBDATAPROC CopyBufferSubData;
  PFNGLGETUNIFORMINDICESPROC GetUniformIndices;
  PFNGLGETACTIVEUNIFORMSIVPROC GetActiveUniformsiv;
  PFNGLGETACTIVEUNIFORMNAMEPROC GetActiveUniformName;
  PFNGLGETUNIFORMBLOCKINDEXPROC GetUniformBlockIndex;
  PFNGLGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
  PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC GetActiveUniformBlockName;
  PFNGLUNIFORMBLOCKBINDINGPROC UniformBlockBinding;
  PFNGLDRAWELEMENTSBASEVERTEXPROC DrawElementsBaseVertex;
  PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC DrawRangeElementsBaseVertex;
  PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC DrawElementsInstancedBaseVertex;
  PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC MultiDrawElementsBaseVertex;
  PFNGLPROVOKINGVERTEXPROC ProvokingVertex;
  PFNGLFENCESYNCPROC FenceSync;
  PFNGLISSYNCPROC IsSync;
  PFNGLDELETESYNCPROC DeleteSync;
  PFNGLCLIENTWAITSYNCPROC ClientWaitSync;
  PFNGLWAITSYNCPROC WaitSync;
  PFNGLGETINTEGER64VPROC GetInteger64v;
  PFNGLGETSYNCIVPROC GetSynciv;
  PFNGLGETINTEGER64I_VPROC GetInteger64i_v;
  PFNGLGETBUFFERPARAMETERI64VPROC GetBufferParameteri64v;
  PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture;
  PFNGLTEXIMAGE2DMULTISAMPLEPROC TexImage2DMultisample;
  PFNGLTEXIMAGE3DMULTISAMPLEPROC TexImage3DMultisample;
  PFNGLGETMULTISAMPLEFVPROC GetMultisamplefv;
  PFNGLSAMPLEMASKIPROC SampleMaski;
  PFNGLBINDFRAGDATALOCATIONINDEXEDPROC BindFragDataLocationIndexed;
  PFNGLGETFRAGDATAINDEXPROC GetFragDataIndex;
  PFNGLGENSAMPLERSPROC GenSamplers;
  PFNGLDELETESAMPLERSPROC DeleteSamplers;
  PFNGLISSAMPLERPROC IsSampler;
  PFNGLBINDSAMPLERPROC BindSampler;
  PFNGLSAMPLERPARAMETERIPROC SamplerParameteri;
  PFNGLSAMPLERPARAMETERIVPROC SamplerParameteriv;
  PFNGLSAMPLERPARAMETERFPROC SamplerParameterf;
  PFNGLSAMPLERPARAMETERFVPROC SamplerParameterfv;
  PFNGLSAMPLERPARAMETERIIVPROC SamplerParameterIiv;
  PFNGLSAMPLERPARAMETERIUIVPROC SamplerParameterIuiv;
  PFNGLGETSAMPLERPARAMETERIVPROC GetSamplerParameteriv;
  PFNGLGETSAMPLERPARAMETERIIVPROC GetSamplerParameterIiv;
  PFNGLGETSAMPLERPARAMETERFVPROC GetSamplerParameterfv;
  PFNGLGETSAMPLERPARAMETERIUIVPROC GetSamplerParameterIuiv;
  PFNGLQUERYCOUNTERPROC QueryCounter;
  PFNGLGETQUERYOBJECTI64VPROC GetQueryObjecti64v;
  PFNGLGETQUERYOBJECTUI64VPROC GetQueryObjectui64v;
  PFNGLVERTEXATTRIBDIVISORPROC VertexAttribDivisor;
  PFNGLVERTEXATTRIBP1UIPROC VertexAttribP1ui;
  PFNGLVERTEXATTRIBP1UIVPROC VertexAttribP1uiv;
  PFNGLVERTEXATTRIBP2UIPROC VertexAttribP2ui;
  PFNGLVERTEXATTRIBP2UIVPROC VertexAttribP2uiv;
  PFNGLVERTEXATTRIBP3UIPROC VertexAttribP3ui;
  PFNGLVERTEXATTRIBP3UIVPROC VertexAttribP3uiv;
  PFNGLVERTEXATTRIBP4UIPROC VertexAttribP4ui;
  PFNGLVERTEXATTRIBP4UIVPROC VertexAttribP4uiv;
  PFNGLMINSAMPLESHADINGPROC MinSampleShading;
  PFNGLBLENDEQUATIONIPROC BlendEquationi;
  PFNGLBLENDEQUATIONSEPARATEIPROC BlendEquationSeparatei;
  PFNGLBLENDFUNCIPROC BlendFunci;
  PFNGLBLENDFUNCSEPARATEIPROC BlendFuncSeparatei;
  PFNGLDRAWARRAYSINDIRECTPROC DrawArraysIndirect;
  PFNGLDRAWELEMENTSINDIRECTPROC DrawElementsIndirect;
  PFNGLUNIFORM1DPROC Uniform1d;
  PFNGLUNIFORM2DPROC Uniform2d;
  PFNGLUNIFORM3DPROC Uniform3d;
  PFNGLUNIFORM4DPROC Uniform4d;
  PFNGLUNIFORM1DVPROC Uniform1dv;
  PFNGLUNIFORM2DVPROC Uniform2dv;
  PFNGLUNIFORM3DVPROC Uniform3dv;
  PFNGLUNIFORM4DVPROC Uniform4dv;
  PFNGLUNIFORMMATRIX2DVPROC UniformMatrix2dv;
  PFNGLUNIFORMMATRIX3DVPROC UniformMatrix3dv;
  PFNGLUNIFORMMATRIX4DVPROC UniformMatrix4dv;
  PFNGLUNIFORMMATRIX2X3DVPROC UniformMatrix2x3dv;
  PFNGLUNIFORMMATRIX2X4DVPROC UniformMatrix2x4dv;
  PFNGLUNIFORMMATRIX3X2DVPROC UniformMatrix3x2dv;
  PFNGLUNIFORMMATRIX3X4DVPROC UniformMatrix3x4dv;
  PFNGLUNIFORMMATRIX4X2DVPROC UniformMatrix4x2dv;
  PFNGLUNIFORMMATRIX4X3DVPROC UniformMatrix4x3dv;
  PFNGLGETUNIFORMDVPROC GetUniformdv;
  PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC GetSubroutineUniformLocation;
  PFNGLGETSUBROUTINEINDEXPROC GetSubroutineIndex;
  PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC GetActiveSubroutineUniformiv;
  PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC GetActiveSubroutineUniformName;
  PFNGLGETACTIVESUBROUTINENAMEPROC GetActiveSubroutineName;
  PFNGLUNIFORMSUBROUTINESUIVPROC UniformSubroutinesuiv;
  PFNGLGETUNIFORMSUBROUTINEUIVPROC GetUniformSubroutineuiv;
  PFNGLGETPROGRAMSTAGEIVPROC GetProgramStageiv;
  PFNGLPATCHPARAMETERIPROC PatchParameteri;
  PFNGLPATCHPARAMETERFVPROC PatchParameterfv;
  PFNGLBINDTRANSFORMFEEDBACKPROC BindTransformFeedback;
  PFNGLDELETETRANSFORMFEEDBACKSPROC DeleteTransformFeedbacks;
  PFNGLGENTRANSFORMFEEDBACKSPROC GenTransformFeedbacks;
  PFNGLISTRANSFORMFEEDBACKPROC IsTransformFeedback;
  PFNGLPAUSETRANSFORMFEEDBACKPROC PauseTransformFeedback;
  PFNGLRESUMETRANSFORMFEEDBACKPROC ResumeTransformFeedback;
  PFNGLDRAWTRANSFORMFEEDBACKPROC DrawTransformFeedback;
  PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC DrawTransformFeedbackStream;
  PFNGLBEGINQUERYINDEXEDPROC BeginQueryIndexed;
  PFNGLENDQUERYINDEXEDPROC EndQueryIndexed;
  PFNGLGETQUERYINDEXEDIVPROC GetQueryIndexediv;
  PFNGLRELEASESHADERCOMPILERPROC ReleaseShaderCompiler;
  PFNGLSHADERBINARYPROC ShaderBinary;
  PFNGLGETSHADERPRECISIONFORMATPROC GetShaderPrecisionFormat;
  PFNGLDEPTHRANGEFPROC DepthRangef;
  PFNGLCLEARDEPTHFPROC ClearDepthf;
  PFNGLGETPROGRAMBINARYPROC GetProgramBinary;
  PFNGLPROGRAMBINARYPROC ProgramBinary;
  PFNGLPROGRAMPARAMETERIPROC ProgramParameteri;
  PFNGLUSEPROGRAMSTAGESPROC UseProgramStages;
  PFNGLACTIVESHADERPROGRAMPROC ActiveShaderProgram;
  PFNGLCREATESHADERPROGRAMVPROC CreateShaderProgramv;
  PFNGLBINDPROGRAMPIPELINEPROC BindProgramPipeline;
  PFNGLDELETEPROGRAMPIPELINESPROC DeleteProgramPipelines;
  PFNGLGENPROGRAMPIPELINESPROC GenProgramPipelines;
  PFNGLISPROGRAMPIPELINEPROC IsProgramPipeline;
  PFNGLGETPROGRAMPIPELINEIVPROC GetProgramPipelineiv;
  PFNGLPROGRAMUNIFORM1IPROC ProgramUniform1i;
  PFNGLPROGRAMUNIFORM1IVPROC ProgramUniform1iv;
  PFNGLPROGRAMUNIFORM1FPROC ProgramUniform1f;
  PFNGLPROGRAMUNIFORM1FVPROC ProgramUniform1fv;
  PFNGLPROGRAMUNIFORM1DPROC ProgramUniform1d;
  PFNGLPROGRAMUNIFORM1DVPROC ProgramUniform1dv;
  PFNGLPROGRAMUNIFORM1UIPROC ProgramUniform1ui;
  PFNGLPROGRAMUNIFORM1UIVPROC ProgramUniform1uiv;
  PFNGLPROGRAMUNIFORM2IPROC ProgramUniform2i;
  PFNGLPROGRAMUNIFORM2IVPROC ProgramUniform2iv;
  PFNGLPROGRAMUNIFORM2FPROC ProgramUniform2f;
  PFNGLPROGRAMUNIFORM2FVPROC ProgramUniform2fv;
  PFNGLPROGRAMUNIFORM2DPROC ProgramUniform2d;
  PFNGLPROGRAMUNIFORM2DVPROC ProgramUniform2dv;
  PFNGLPROGRAMUNIFORM2UIPROC ProgramUniform2ui;
  PFNGLPROGRAMUNIFORM2UIVPROC ProgramUniform2uiv;
  PFNGLPROGRAMUNIFORM3IPROC ProgramUniform3i;
  PFNGLPROGRAMUNIFORM3IVPROC ProgramUniform3iv;
  PFNGLPROGRAMUNIFORM3FPROC ProgramUniform3f;
  PFNGLPROGRAMUNIFORM3FVPROC ProgramUniform3fv;
  PFNGLPROGRAMUNIFORM3DPROC ProgramUniform3d;
  PFNGLPROGRAMUNIFORM3DVPROC ProgramUniform3dv;
  PFNGLPROGRAMUNIFORM3UIPROC ProgramUniform3ui;
  PFNGLPROGRAMUNIFORM3UIVPROC ProgramUniform3uiv;
  PFNGLPROGRAMUNIFORM4IPROC ProgramUniform4i;
  PFNGLPROGRAMUNIFORM4IVPROC ProgramUniform4iv;
  PFNGLPROGRAMUNIFORM4FPROC ProgramUniform4f;
  PFNGLPROGRAMUNIFORM4FVPROC ProgramUniform4fv;
  PFNGLPROGRAMUNIFORM4DPROC ProgramUniform4d;
  PFNGLPROGRAMUNIFORM4DVPROC ProgramUniform4dv;
  PFNGLPROGRAMUNIFORM4UIPROC ProgramUniform4ui;
  PFNGLPROGRAMUNIFORM4UIVPROC ProgramUniform4uiv;
  PFNGLPROGRAMUNIFORMMATRIX2FVPROC ProgramUniformMatrix2fv;
  PFNGLPROGRAMUNIFORMMATRIX3FVPROC ProgramUniformMatrix3fv;
  PFNGLPROGRAMUNIFORMMATRIX4FVPROC ProgramUniformMatrix4fv;
  PFNGLPROGRAMUNIFORMMATRIX2DVPROC ProgramUniformMatrix2dv;
  PFNGLPROGRAMUNIFORMMATRIX3DVPROC ProgramUniformMatrix3dv;
  PFNGLPROGRAMUNIFORMMATRIX4DVPROC ProgramUniformMatrix4dv;
  PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC ProgramUniformMatrix2x3fv;
  PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC ProgramUniformMatrix3x2fv;
  PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC ProgramUniformMatrix2x4fv;
  PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC ProgramUniformMatrix4x2fv;
  PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC ProgramUniformMatrix3x4fv;
  PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC ProgramUniformMatrix4x3fv;
  PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC ProgramUniformMatrix2x3dv;
  PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC ProgramUniformMatrix3x2dv;
  PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC ProgramUniformMatrix2x4dv;
  PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC ProgramUniformMatrix4x2dv;
  PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC ProgramUniformMatrix3x4dv;
  PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC ProgramUniformMatrix4x3dv;
  PFNGLVALIDATEPROGRAMPIPELINEPROC ValidateProgramPipeline;
  PFNGLGETPROGRAMPIPELINEINFOLOGPROC GetProgramPipelineInfoLog;
  PFNGLVERTEXATTRIBL1DPROC VertexAttribL1d;
  PFNGLVERTEXATTRIBL2DPROC VertexAttribL2d;
  PFNGLVERTEXATTRIBL3DPROC VertexAttribL3d;
  PFNGLVERTEXATTRIBL4DPROC VertexAttribL4d;
  PFNGLVERTEXATTRIBL1DVPROC VertexAttribL1dv;
  PFNGLVERTEXATTRIBL2DVPROC VertexAttribL2dv;
  PFNGLVERTEXATTRIBL3DVPROC VertexAttribL3dv;
  PFNGLVERTEXATTRIBL4DVPROC VertexAttribL4dv;
  PFNGLVERTEXATTRIBLPOINTERPROC VertexAttribLPointer;
  PFNGLGETVERTEXATTRIBLDVPROC GetVertexAttribLdv;
  PFNGLVIEWPORTARRAYVPROC ViewportArrayv;
  PFNGLVIEWPORTINDEXEDFPROC ViewportIndexedf;
  PFNGLVIEWPORTINDEXEDFVPROC ViewportIndexedfv;
  PFNGLSCISSORARRAYVPROC ScissorArrayv;
  PFNGLSCISSORINDEXEDPROC ScissorIndexed;
  PFNGLSCISSORINDEXEDVPROC ScissorIndexedv;
  PFNGLDEPTHRANGEARRAYVPROC DepthRangeArrayv;
  PFNGLDEPTHRANGEINDEXEDPROC DepthRangeIndexed;
  PFNGLGETFLOATI_VPROC GetFloati_v;
  PFNGLGETDOUBLEI_VPROC GetDoublei_v;
  PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC DrawArraysInstancedBaseInstance;
  PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC DrawElementsInstancedBaseInstance;
  PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC DrawElementsInstancedBaseVertexBaseInstance;
  PFNGLGETINTERNALFORMATIVPROC GetInternalformativ;
  PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC GetActiveAtomicCounterBufferiv;
  PFNGLBINDIMAGETEXTUREPROC BindImageTexture;
  PFNGLMEMORYBARRIERPROC MemoryBarrier;
  PFNGLTEXSTORAGE1DPROC TexStorage1D;
  PFNGLTEXSTORAGE2DPROC TexStorage2D;
  PFNGLTEXSTORAGE3DPROC TexStorage3D;
  PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC DrawTransformFeedbackInstanced;
  PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC DrawTransformFeedbackStreamInstanced;
  PFNGLCLEARBUFFERDATAPROC ClearBufferData;
  PFNGLCLEARBUFFERSUBDATAPROC ClearBufferSubData;
  PFNGLDISPATCHCOMPUTEPROC DispatchCompute;
  PFNGLDISPATCHCOMPUTEINDIRECTPROC DispatchComputeIndirect;
  PFNGLCOPYIMAGESUBDATAPROC CopyImageSubData;
  PFNGLFRAMEBUFFERPARAMETERIPROC FramebufferParameteri;
  PFNGLGETFRAMEBUFFERPARAMETERIVPROC GetFramebufferParameteriv;
  PFNGLGETINTERNALFORMATI64VPROC GetInternalformati64v;
  PFNGLINVALIDATETEXSUBIMAGEPROC InvalidateTexSubImage;
  PFNGLINVALIDATETEXIMAGEPROC InvalidateTexImage;
  PFNGLINVALIDATEBUFFERSUBDATAPROC InvalidateBufferSubData;
  PFNGLINVALIDATEBUFFERDATAPROC InvalidateBufferData;
  PFNGLINVALIDATEFRAMEBUFFERPROC InvalidateFramebuffer;
  PFNGLINVALIDATESUBFRAMEBUFFERPROC InvalidateSubFramebuffer;
  PFNGLMULTIDRAWARRAYSINDIRECTPROC MultiDrawArraysIndirect;
  PFNGLMULTIDRAWELEMENTSINDIRECTPROC MultiDrawElementsIndirect;
  PFNGLGETPROGRAMINTERFACEIVPROC GetProgramInterfaceiv;
  PFNGLGETPROGRAMRESOURCEINDEXPROC GetProgramResourceIndex;
  PFNGLGETPROGRAMRESOURCENAMEPROC GetProgramResourceName;
  PFNGLGETPROGRAMRESOURCEIVPROC GetProgramResourceiv;
  PFNGLGETPROGRAMRESOURCELOCATIONPROC GetProgramResourceLocation;
  PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC GetProgramResourceLocationIndex;
  PFNGLSHADERSTORAGEBLOCKBINDINGPROC ShaderStorageBlockBinding;
  PFNGLTEXBUFFERRANGEPROC TexBufferRange;
  PFNGLTEXSTORAGE2DMULTISAMPLEPROC TexStorage2DMultisample;
  PFNGLTEXSTORAGE3DMULTISAMPLEPROC TexStorage3DMultisample;
  PFNGLTEXTUREVIEWPROC TextureView;
  PFNGLBINDVERTEXBUFFERPROC BindVertexBuffer;
  PFNGLVERTEXATTRIBFORMATPROC VertexAttribFormat;
  PFNGLVERTEXATTRIBIFORMATPROC VertexAttribIFormat;
  PFNGLVERTEXATTRIBLFORMATPROC VertexAttribLFormat;
  PFNGLVERTEXATTRIBBINDINGPROC VertexAttribBinding;
  PFNGLVERTEXBINDINGDIVISORPROC VertexBindingDivisor;
  PFNGLDEBUGMESSAGECONTROLPROC DebugMessageControl;
  PFNGLDEBUGMESSAGEINSERTPROC DebugMessageInsert;
  PFNGLDEBUGMESSAGECALLBACKPROC DebugMessageCallback;
  PFNGLGETDEBUGMESSAGELOGPROC GetDebugMessageLog;
  PFNGLPUSHDEBUGGROUPPROC PushDebugGroup;
  PFNGLPOPDEBUGGROUPPROC PopDebugGroup;
  PFNGLOBJECTLABELPROC ObjectLabel;
  PFNGLGETOBJECTLABELPROC GetObjectLabel;
  PFNGLOBJECTPTRLABELPROC ObjectPtrLabel;
  PFNGLGETOBJECTPTRLABELPROC GetObjectPtrLabel;
  PFNGLBUFFERSTORAGEPROC BufferStorage;
  PFNGLCLEARTEXIMAGEPROC ClearTexImage;
  PFNGLCLEARTEXSUBIMAGEPROC ClearTexSubImage;
  PFNGLBINDBUFFERSBASEPROC BindBuffersBase;
  PFNGLBINDBUFFERSRANGEPROC BindBuffersRange;
  PFNGLBINDTEXTURESPROC BindTextures;
  PFNGLBINDSAMPLERSPROC BindSamplers;
  PFNGLBINDIMAGETEXTURESPROC BindImageTextures;
  PFNGLBINDVERTEXBUFFERSPROC BindVertexBuffers;
  PFNGLCLIPCONTROLPROC ClipControl;
  PFNGLCREATETRANSFORMFEEDBACKSPROC CreateTransformFeedbacks;
  PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC TransformFeedbackBufferBase;
  PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC TransformFeedbackBufferRange;
  PFNGLGETTRANSFORMFEEDBACKIVPROC GetTransformFeedbackiv;
  PFNGLGETTRANSFORMFEEDBACKI_VPROC GetTransformFeedbacki_v;
  PFNGLGETTRANSFORMFEEDBACKI64_VPROC GetTransformFeedbacki64_v;
  PFNGLCREATEBUFFERSPROC CreateBuffers;
  PFNGLNAMEDBUFFERSTORAGEPROC NamedBufferStorage;
  PFNGLNAMEDBUFFERDATAPROC NamedBufferData;
  PFNGLNAMEDBUFFERSUBDATAPROC NamedBufferSubData;
  PFNGLCOPYNAMEDBUFFERSUBDATAPROC CopyNamedBufferSubData;
  PFNGLCLEARNAMEDBUFFERDATAPROC ClearNamedBufferData;
  PFNGLCLEARNAMEDBUFFERSUBDATAPROC ClearNamedBufferSubData;
  PFNGLMAPNAMEDBUFFERPROC MapNamedBuffer;
  PFNGLMAPNAMEDBUFFERRANGEPROC MapNamedBufferRange;
  PFNGLUNMAPNAMEDBUFFERPROC UnmapNamedBuffer;
  PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC FlushMappedNamedBufferRange;
  PFNGLGETNAMEDBUFFERPARAMETERIVPROC GetNamedBufferParameteriv;
  PFNGLGETNAMEDBUFFERPARAMETERI64VPROC GetNamedBufferParameteri64v;
  PFNGLGETNAMEDBUFFERPOINTERVPROC GetNamedBufferPointerv;
  PFNGLGETNAMEDBUFFERSUBDATAPROC GetNamedBufferSubData;
  PFNGLCREATEFRAMEBUFFERSPROC CreateFramebuffers;
  PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC NamedFramebufferRenderbuffer;
  PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC NamedFramebufferParameteri;
  PFNGLNAMEDFRAMEBUFFERTEXTUREPROC NamedFramebufferTexture;
  PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC NamedFramebufferTextureLayer;
  PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC NamedFramebufferDrawBuffer;
  PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC NamedFramebufferDrawBuffers;
  PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC NamedFramebufferReadBuffer;
  PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC InvalidateNamedFramebufferData;
  PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC InvalidateNamedFramebufferSubData;
  PFNGLCLEARNAMEDFRAMEBUFFERIVPROC ClearNamedFramebufferiv;
  PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC ClearNamedFramebufferuiv;
  PFNGLCLEARNAMEDFRAMEBUFFERFVPROC ClearNamedFramebufferfv;
  PFNGLCLEARNAMEDFRAMEBUFFERFIPROC ClearNamedFramebufferfi;
  PFNGLBLITNAMEDFRAMEBUFFERPROC BlitNamedFramebuffer;
  PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC CheckNamedFramebufferStatus;
  PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC GetNamedFramebufferParameteriv;
  PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetNamedFramebufferAttachmentParameteriv;
  PFNGLCREATERENDERBUFFERSPROC CreateRenderbuffers;
  PFNGLNAMEDRENDERBUFFERSTORAGEPROC NamedRenderbufferStorage;
  PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC NamedRenderbufferStorageMultisample;
  PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC GetNamedRenderbufferParameteriv;
  PFNGLCREATETEXTURESPROC CreateTextures;
  PFNGLTEXTUREBUFFERPROC TextureBuffer;
  PFNGLTEXTUREBUFFERRANGEPROC TextureBufferRange;
  PFNGLTEXTURESTORAGE1DPROC TextureStorage1D;
  PFNGLTEXTURESTORAGE2DPROC TextureStorage2D;
  PFNGLTEXTURESTORAGE3DPROC TextureStorage3D;
  PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC TextureStorage2DMultisample;
  PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC TextureStorage3DMultisample;
  PFNGLTEXTURESUBIMAGE1DPROC TextureSubImage1D;
  PFNGLTEXTURESUBIMAGE2DPROC TextureSubImage2D;
  PFNGLTEXTURESUBIMAGE3DPROC TextureSubImage3D;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC CompressedTextureSubImage1D;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC CompressedTextureSubImage2D;
  PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC CompressedTextureSubImage3D;
  PFNGLCOPYTEXTURESUBIMAGE1DPROC CopyTextureSubImage1D;
  PFNGLCOPYTEXTURESUBIMAGE2DPROC CopyTextureSubImage2D;
  PFNGLCOPYTEXTURESUBIMAGE3DPROC CopyTextureSubImage3D;
  PFNGLTEXTUREPARAMETERFPROC TextureParameterf;
  PFNGLTEXTUREPARAMETERFVPROC TextureParameterfv;
  PFNGLTEXTUREPARAMETERIPROC TextureParameteri;
  PFNGLTEXTUREPARAMETERIIVPROC TextureParameterIiv;
  PFNGLTEXTUREPARAMETERIUIVPROC TextureParameterIuiv;
  PFNGLTEXTUREPARAMETERIVPROC TextureParameteriv;
  PFNGLGENERATETEXTUREMIPMAPPROC GenerateTextureMipmap;
  PFNGLBINDTEXTUREUNITPROC BindTextureUnit;
  PFNGLGETTEXTUREIMAGEPROC GetTextureImage;
  PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC GetCompressedTextureImage;
  PFNGLGETTEXTURELEVELPARAMETERFVPROC GetTextureLevelParameterfv;
  PFNGLGETTEXTURELEVELPARAMETERIVPROC GetTextureLevelParameteriv;
  PFNGLGETTEXTUREPARAMETERFVPROC GetTextureParameterfv;
  PFNGLGETTEXTUREPARAMETERIIVPROC GetTextureParameterIiv;
  PFNGLGETTEXTUREPARAMETERIUIVPROC GetTextureParameterIuiv;
  PFNGLGETTEXTUREPARAMETERIVPROC GetTextureParameteriv;
  PFNGLCREATEVERTEXARRAYSPROC CreateVertexArrays;
  PFNGLDISABLEVERTEXARRAYATTRIBPROC DisableVertexArrayAttrib;
  PFNGLENABLEVERTEXARRAYATTRIBPROC EnableVertexArrayAttrib;
  PFNGLVERTEXARRAYELEMENTBUFFERPROC VertexArrayElementBuffer;
  PFNGLVERTEXARRAYVERTEXBUFFERPROC VertexArrayVertexBuffer;
  PFNGLVERTEXARRAYVERTEXBUFFERSPROC VertexArrayVertexBuffers;
  PFNGLVERTEXARRAYATTRIBBINDINGPROC VertexArrayAttribBinding;
  PFNGLVERTEXARRAYATTRIBFORMATPROC VertexArrayAttribFormat;
  PFNGLVERTEXARRAYATTRIBIFORMATPROC VertexArrayAttribIFormat;
  PFNGLVERTEXARRAYATTRIBLFORMATPROC VertexArrayAttribLFormat;
  PFNGLVERTEXARRAYBINDINGDIVISORPROC VertexArrayBindingDivisor;
  PFNGLGETVERTEXARRAYIVPROC GetVertexArrayiv;
  PFNGLGETVERTEXARRAYINDEXEDIVPROC GetVertexArrayIndexediv;
  PFNGLGETVERTEXARRAYINDEXED64IVPROC GetVertexArrayIndexed64iv;
  PFNGLCREATESAMPLERSPROC CreateSamplers;
  PFNGLCREATEPROGRAMPIPELINESPROC CreateProgramPipelines;
  PFNGLCREATEQUERIESPROC CreateQueries;
  PFNGLGETQUERYBUFFEROBJECTI64VPROC GetQueryBufferObjecti64v;
  PFNGLGETQUERYBUFFEROBJECTIVPROC GetQueryBufferObjectiv;
  PFNGLGETQUERYBUFFEROBJECTUI64VPROC GetQueryBufferObjectui64v;
  PFNGLGETQUERYBUFFEROBJECTUIVPROC GetQueryBufferObjectuiv;
  PFNGLMEMORYBARRIERBYREGIONPROC MemoryBarrierByRegion;
  PFNGLGETTEXTURESUBIMAGEPROC GetTextureSubImage;
  PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC GetCompressedTextureSubImage;
  PFNGLGETGRAPHICSRESETSTATUSPROC GetGraphicsResetStatus;
  PFNGLGETNCOMPRESSEDTEXIMAGEPROC GetnCompressedTexImage;
  PFNGLGETNTEXIMAGEPROC GetnTexImage;
  PFNGLGETNUNIFORMDVPROC GetnUniformdv;
  PFNGLGETNUNIFORMFVPROC GetnUniformfv;
  PFNGLGETNUNIFORMIVPROC GetnUniformiv;
  PFNGLGETNUNIFORMUIVPROC GetnUniformuiv;
  PFNGLREADNPIXELSPROC ReadnPixels;
  PFNGLTEXTUREBARRIERPROC TextureBarrier;
  PFNGLSPECIALIZESHADERPROC SpecializeShader;
  PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC MultiDrawArraysIndirectCount;
  PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC MultiDrawElementsIndirectCount;
  PFNGLPOLYGONOFFSETCLAMPPROC PolygonOffsetClamp;
};

const char* const GL_FUNCTIONS[] = {"glCullFace",
                                    "glFrontFace",
                                    "glHint",
                                    "glLineWidth",
                                    "glPointSize",
                                    "glPolygonMode",
                                    "glScissor",
                                    "glTexParameterf",
                                    "glTexParameterfv",
                                    "glTexParameteri",
                                    "glTexParameteriv",
                                    "glTexImage1D",
                                    "glTexImage2D",
                                    "glDrawBuffer",
                                    "glClear",
                                    "glClearColor",
                                    "glClearStencil",
                                    "glClearDepth",
                                    "glStencilMask",
                                    "glColorMask",
                                    "glDepthMask",
                                    "glDisable",
                                    "glEnable",
                                    "glFinish",
                                    "glFlush",
                                    "glBlendFunc",
                                    "glLogicOp",
                                    "glStencilFunc",
                                    "glStencilOp",
                                    "glDepthFunc",
                                    "glPixelStoref",
                                    "glPixelStorei",
                                    "glReadBuffer",
                                    "glReadPixels",
                                    "glGetBooleanv",
                                    "glGetDoublev",
                                    "glGetError",
                                    "glGetFloatv",
                                    "glGetIntegerv",
                                    "glGetString",
                                    "glGetTexImage",
                                    "glGetTexParameterfv",
                                    "glGetTexParameteriv",
                                    "glGetTexLevelParameterfv",
                                    "glGetTexLevelParameteriv",
                                    "glIsEnabled",
                                    "glDepthRange",
                                    "glViewport",
                                    "glDrawArrays",
                                    "glDrawElements",
                                    "glGetPointerv",
                                    "glPolygonOffset",
                                    "glCopyTexImage1D",
                                    "glCopyTexImage2D",
                                    "glCopyTexSubImage1D",
                                    "glCopyTexSubImage2D",
                                    "glTexSubImage1D",
                                    "glTexSubImage2D",
                                    "glBindTexture",
                                    "glDeleteTextures",
                                    "glGenTextures",
                                    "glIsTexture",
                                    "glDrawRangeElements",
                                    "glTexImage3D",
                                    "glTexSubImage3D",
                                    "glCopyTexSubImage3D",
                                    "glActiveTexture",
                                    "glSampleCoverage",
                                    "glCompressedTexImage3D",
                                    "glCompressedTexImage2D",
                                    "glCompressedTexImage1D",
                                    "glCompressedTexSubImage3D",
                                    "glCompressedTexSubImage2D",
                                    "glCompressedTexSubImage1D",
                                    "glGetCompressedTexImage",
                                    "glBlendFuncSeparate",
                                    "glMultiDrawArrays",
                                    "glMultiDrawElements",
                                    "glPointParameterf",
                                    "glPointParameterfv",
                                    "glPointParameteri",
                                    "glPointParameteriv",
                                    "glBlendColor",
                                    "glBlendEquation",
                                    "glGenQueries",
                                    "glDeleteQueries",
                                    "glIsQuery",
                                    "glBeginQuery",
                                    "glEndQuery",
                                    "glGetQueryiv",
                                    "glGetQueryObjectiv",
                                    "glGetQueryObjectuiv",
                                    "glBindBuffer",
                                    "glDeleteBuffers",
                                    "glGenBuffers",
                                    "glIsBuffer",
                                    "glBufferData",
                                    "glBufferSubData",
                                    "glGetBufferSubData",
                                    "glMapBuffer",
                                    "glUnmapBuffer",
                                    "glGetBufferParameteriv",
                                    "glGetBufferPointerv",
                                    "glBlendEquationSeparate",
                                    "glDrawBuffers",
                                    "glStencilOpSeparate",
                                    "glStencilFuncSeparate",
                                    "glStencilMaskSeparate",
                                    "glAttachShader",
                                    "glBindAttribLocation",
                                    "glCompileShader",
                                    "glCreateProgram",
                                    "glCreateShader",
                                    "glDeleteProgram",
                                    "glDeleteShader",
                                    "glDetachShader",
                                    "glDisableVertexAttribArray",
                                    "glEnableVertexAttribArray",
                                    "glGetActiveAttrib",
                                    "glGetActiveUniform",
                                    "glGetAttachedShaders",
                                    "glGetAttribLocation",
                                    "glGetProgramiv",
                                    "glGetProgramInfoLog",
                                    "glGetShaderiv",
                                    "glGetShaderInfoLog",
                                    "glGetShaderSource",
                                    "glGetUniformLocation",
                                    "glGetUniformfv",
                                    "glGetUniformiv",
                                    "glGetVertexAttribdv",
                                    "glGetVertexAttribfv",
                                    "glGetVertexAttribiv",
                                    "glGetVertexAttribPointerv",
                                    "glIsProgram",
                                    "glIsShader",
                                    "glLinkProgram",
                                    "glShaderSource",
                                    "glUseProgram",
                                    "glUniform1f",
                                    "glUniform2f",
                                    "glUniform3f",
                                    "glUniform4f",
                                    "glUniform1i",
                                    "glUniform2i",
                                    "glUniform3i",
                                    "glUniform4i",
                                    "glUniform1fv",
                                    "glUniform2fv",
                                    "glUniform3fv",
                                    "glUniform4fv",
                                    "glUniform1iv",
                                    "glUniform2iv",
                                    "glUniform3iv",
                                    "glUniform4iv",
                                    "glUniformMatrix2fv",
                                    "glUniformMatrix3fv",
                                    "glUniformMatrix4fv",
                                    "glValidateProgram",
                                    "glVertexAttrib1d",
                                    "glVertexAttrib1dv",
                                    "glVertexAttrib1f",
                                    "glVertexAttrib1fv",
                                    "glVertexAttrib1s",
                                    "glVertexAttrib1sv",
                                    "glVertexAttrib2d",
                                    "glVertexAttrib2dv",
                                    "glVertexAttrib2f",
                                    "glVertexAttrib2fv",
                                    "glVertexAttrib2s",
                                    "glVertexAttrib2sv",
                                    "glVertexAttrib3d",
                                    "glVertexAttrib3dv",
                                    "glVertexAttrib3f",
                                    "glVertexAttrib3fv",
                                    "glVertexAttrib3s",
                                    "glVertexAttrib3sv",
                                    "glVertexAttrib4Nbv",
                                    "glVertexAttrib4Niv",
                                    "glVertexAttrib4Nsv",
                                    "glVertexAttrib4Nub",
                                    "glVertexAttrib4Nubv",
                                    "glVertexAttrib4Nuiv",
                                    "glVertexAttrib4Nusv",
                                    "glVertexAttrib4bv",
                                    "glVertexAttrib4d",
                                    "glVertexAttrib4dv",
                                    "glVertexAttrib4f",
                                    "glVertexAttrib4fv",
                                    "glVertexAttrib4iv",
                                    "glVertexAttrib4s",
                                    "glVertexAttrib4sv",
                                    "glVertexAttrib4ubv",
                                    "glVertexAttrib4uiv",
                                    "glVertexAttrib4usv",
                                    "glVertexAttribPointer",
                                    "glUniformMatrix2x3fv",
                                    "glUniformMatrix3x2fv",
                                    "glUniformMatrix2x4fv",
                                    "glUniformMatrix4x2fv",
                                    "glUniformMatrix3x4fv",
                                    "glUniformMatrix4x3fv",
                                    "glColorMaski",
                                    "glGetBooleani_v",
                                    "glGetIntegeri_v",
                                    "glEnablei",
                                    "glDisablei",
                                    "glIsEnabledi",
                                    "glBeginTransformFeedback",
                                    "glEndTransformFeedback",
                                    "glBindBufferRange",
                                    "glBindBufferBase",
                                    "glTransformFeedbackVaryings",
                                    "glGetTransformFeedbackVarying",
                                    "glClampColor",
                                    "glBeginConditionalRender",
                                    "glEndConditionalRender",
                                    "glVertexAttribIPointer",
                                    "glGetVertexAttribIiv",
                                    "glGetVertexAttribIuiv",
                                    "glVertexAttribI1i",
                                    "glVertexAttribI2i",
                                    "glVertexAttribI3i",
                                    "glVertexAttribI4i",
                                    "glVertexAttribI1ui",
                                    "glVertexAttribI2ui",
                                    "glVertexAttribI3ui",
                                    "glVertexAttribI4ui",
                                    "glVertexAttribI1iv",
                                    "glVertexAttribI2iv",
                                    "glVertexAttribI3iv",
                                    "glVertexAttribI4iv",
                                    "glVertexAttribI1uiv",
                                    "glVertexAttribI2uiv",
                                    "glVertexAttribI3uiv",
                                    "glVertexAttribI4uiv",
                                    "glVertexAttribI4bv",
                                    "glVertexAttribI4sv",
                                    "glVertexAttribI4ubv",
                                    "glVertexAttribI4usv",
                                    "glGetUniformuiv",
                                    "glBindFragDataLocation",
                                    "glGetFragDataLocation",
                                    "glUniform1ui",
                                    "glUniform2ui",
                                    "glUniform3ui",
                                    "glUniform4ui",
                                    "glUniform1uiv",
                                    "glUniform2uiv",
                                    "glUniform3uiv",
                                    "glUniform4uiv",
                                    "glTexParameterIiv",
                                    "glTexParameterIuiv",
                                    "glGetTexParameterIiv",
                                    "glGetTexParameterIuiv",
                                    "glClearBufferiv",
                                    "glClearBufferuiv",
                                    "glClearBufferfv",
                                    "glClearBufferfi",
                                    "glGetStringi",
                                    "glIsRenderbuffer",
                                    "glBindRenderbuffer",
                                    "glDeleteRenderbuffers",
                                    "glGenRenderbuffers",
                                    "glRenderbufferStorage",
                                    "glGetRenderbufferParameteriv",
                                    "glIsFramebuffer",
                                    "glBindFramebuffer",
                                    "glDeleteFramebuffers",
                                    "glGenFramebuffers",
                                    "glCheckFramebufferStatus",
                                    "glFramebufferTexture1D",
                                    "glFramebufferTexture2D",
                                    "glFramebufferTexture3D",
                                    "glFramebufferRenderbuffer",
                                    "glGetFramebufferAttachmentParameteriv",
                                    "glGenerateMipmap",
                                    "glBlitFramebuffer",
                                    "glRenderbufferStorageMultisample",
                                    "glFramebufferTextureLayer",
                                    "glMapBufferRange",
                                    "glFlushMappedBufferRange",
                                    "glBindVertexArray",
                                    "glDeleteVertexArrays",
                                    "glGenVertexArrays",
                                    "glIsVertexArray",
                                    "glDrawArraysInstanced",
                                    "glDrawElementsInstanced",
                                    "glTexBuffer",
                                    "glPrimitiveRestartIndex",
                                    "glCopyBufferSubData",
                                    "glGetUniformIndices",
                                    "glGetActiveUniformsiv",
                                    "glGetActiveUniformName",
                                    "glGetUniformBlockIndex",
                                    "glGetActiveUniformBlockiv",
                                    "glGetActiveUniformBlockName",
                                    "glUniformBlockBinding",
                                    "glDrawElementsBaseVertex",
                                    "glDrawRangeElementsBaseVertex",
                                    "glDrawElementsInstancedBaseVertex",
                                    "glMultiDrawElementsBaseVertex",
                                    "glProvokingVertex",
                                    "glFenceSync",
                                    "glIsSync",
                                    "glDeleteSync",
                                    "glClientWaitSync",
                                    "glWaitSync",
                                    "glGetInteger64v",
                                    "glGetSynciv",
                                    "glGetInteger64i_v",
                                    "glGetBufferParameteri64v",
                                    "glFramebufferTexture",
                                    "glTexImage2DMultisample",
                                    "glTexImage3DMultisample",
                                    "glGetMultisamplefv",
                                    "glSampleMaski",
                                    "glBindFragDataLocationIndexed",
                                    "glGetFragDataIndex",
                                    "glGenSamplers",
                                    "glDeleteSamplers",
                                    "glIsSampler",
                                    "glBindSampler",
                                    "glSamplerParameteri",
                                    "glSamplerParameteriv",
                                    "glSamplerParameterf",
                                    "glSamplerParameterfv",
                                    "glSamplerParameterIiv",
                                    "glSamplerParameterIuiv",
                                    "glGetSamplerParameteriv",
                                    "glGetSamplerParameterIiv",
                                    "glGetSamplerParameterfv",
                                    "glGetSamplerParameterIuiv",
                                    "glQueryCounter",
                                    "glGetQueryObjecti64v",
                                    "glGetQueryObjectui64v",
                                    "glVertexAttribDivisor",
                                    "glVertexAttribP1ui",
                                    "glVertexAttribP1uiv",
                                    "glVertexAttribP2ui",
                                    "glVertexAttribP2uiv",
                                    "glVertexAttribP3ui",
                                    "glVertexAttribP3uiv",
                                    "glVertexAttribP4ui",
                                    "glVertexAttribP4uiv",
                                    "glMinSampleShading",
                                    "glBlendEquationi",
                                    "glBlendEquationSeparatei",
                                    "glBlendFunci",
                                    "glBlendFuncSeparatei",
                                    "glDrawArraysIndirect",
                                    "glDrawElementsIndirect",
                                    "glUniform1d",
                                    "glUniform2d",
                                    "glUniform3d",
                                    "glUniform4d",
                                    "glUniform1dv",
                                    "glUniform2dv",
                                    "glUniform3dv",
                                    "glUniform4dv",
                                    "glUniformMatrix2dv",
                                    "glUniformMatrix3dv",
                                    "glUniformMatrix4dv",
                                    "glUniformMatrix2x3dv",
                                    "glUniformMatrix2x4dv",
                                    "glUniformMatrix3x2dv",
                                    "glUniformMatrix3x4dv",
                                    "glUniformMatrix4x2dv",
                                    "glUniformMatrix4x3dv",
                                    "glGetUniformdv",
                                    "glGetSubroutineUniformLocation",
                                    "glGetSubroutineIndex",
                                    "glGetActiveSubroutineUniformiv",
                                    "glGetActiveSubroutineUniformName",
                                    "glGetActiveSubroutineName",
                                    "glUniformSubroutinesuiv",
                                    "glGetUniformSubroutineuiv",
                                    "glGetProgramStageiv",
                                    "glPatchParameteri",
                                    "glPatchParameterfv",
                                    "glBindTransformFeedback",
                                    "glDeleteTransformFeedbacks",
                                    "glGenTransformFeedbacks",
                                    "glIsTransformFeedback",
                                    "glPauseTransformFeedback",
                                    "glResumeTransformFeedback",
                                    "glDrawTransformFeedback",
                                    "glDrawTransformFeedbackStream",
                                    "glBeginQueryIndexed",
                                    "glEndQueryIndexed",
                                    "glGetQueryIndexediv",
                                    "glReleaseShaderCompiler",
                                    "glShaderBinary",
                                    "glGetShaderPrecisionFormat",
                                    "glDepthRangef",
                                    "glClearDepthf",
                                    "glGetProgramBinary",
                                    "glProgramBinary",
                                    "glProgramParameteri",
                                    "glUseProgramStages",
                                    "glActiveShaderProgram",
                                    "glCreateShaderProgramv",
                                    "glBindProgramPipeline",
                                    "glDeleteProgramPipelines",
                                    "glGenProgramPipelines",
                                    "glIsProgramPipeline",
                                    "glGetProgramPipelineiv",
                                    "glProgramUniform1i",
                                    "glProgramUniform1iv",
                                    "glProgramUniform1f",
                                    "glProgramUniform1fv",
                                    "glProgramUniform1d",
                                    "glProgramUniform1dv",
                                    "glProgramUniform1ui",
                                    "glProgramUniform1uiv",
                                    "glProgramUniform2i",
                                    "glProgramUniform2iv",
                                    "glProgramUniform2f",
                                    "glProgramUniform2fv",
                                    "glProgramUniform2d",
                                    "glProgramUniform2dv",
                                    "glProgramUniform2ui",
                                    "glProgramUniform2uiv",
                                    "glProgramUniform3i",
                                    "glProgramUniform3iv",
                                    "glProgramUniform3f",
                                    "glProgramUniform3fv",
                                    "glProgramUniform3d",
                                    "glProgramUniform3dv",
                                    "glProgramUniform3ui",
                                    "glProgramUniform3uiv",
                                    "glProgramUniform4i",
                                    "glProgramUniform4iv",
                                    "glProgramUniform4f",
                                    "glProgramUniform4fv",
                                    "glProgramUniform4d",
                                    "glProgramUniform4dv",
                                    "glProgramUniform4ui",
                                    "glProgramUniform4uiv",
                                    "glProgramUniformMatrix2fv",
                                    "glProgramUniformMatrix3fv",
                                    "glProgramUniformMatrix4fv",
                                    "glProgramUniformMatrix2dv",
                                    "glProgramUniformMatrix3dv",
                                    "glProgramUniformMatrix4dv",
                                    "glProgramUniformMatrix2x3fv",
                                    "glProgramUniformMatrix3x2fv",
                                    "glProgramUniformMatrix2x4fv",
                                    "glProgramUniformMatrix4x2fv",
                                    "glProgramUniformMatrix3x4fv",
                                    "glProgramUniformMatrix4x3fv",
                                    "glProgramUniformMatrix2x3dv",
                                    "glProgramUniformMatrix3x2dv",
                                    "glProgramUniformMatrix2x4dv",
                                    "glProgramUniformMatrix4x2dv",
                                    "glProgramUniformMatrix3x4dv",
                                    "glProgramUniformMatrix4x3dv",
                                    "glValidateProgramPipeline",
                                    "glGetProgramPipelineInfoLog",
                                    "glVertexAttribL1d",
                                    "glVertexAttribL2d",
                                    "glVertexAttribL3d",
                                    "glVertexAttribL4d",
                                    "glVertexAttribL1dv",
                                    "glVertexAttribL2dv",
                                    "glVertexAttribL3dv",
                                    "glVertexAttribL4dv",
                                    "glVertexAttribLPointer",
                                    "glGetVertexAttribLdv",
                                    "glViewportArrayv",
                                    "glViewportIndexedf",
                                    "glViewportIndexedfv",
                                    "glScissorArrayv",
                                    "glScissorIndexed",
                                    "glScissorIndexedv",
                                    "glDepthRangeArrayv",
                                    "glDepthRangeIndexed",
                                    "glGetFloati_v",
                                    "glGetDoublei_v",
                                    "glDrawArraysInstancedBaseInstance",
                                    "glDrawElementsInstancedBaseInstance",
                                    "glDrawElementsInstancedBaseVertexBaseInstance",
                                    "glGetInternalformativ",
                                    "glGetActiveAtomicCounterBufferiv",
                                    "glBindImageTexture",
                                    "glMemoryBarrier",
                                    "glTexStorage1D",
                                    "glTexStorage2D",
                                    "glTexStorage3D",
                                    "glDrawTransformFeedbackInstanced",
                                    "glDrawTransformFeedbackStreamInstanced",
                                    "glClearBufferData",
                                    "glClearBufferSubData",
                                    "glDispatchCompute",
                                    "glDispatchComputeIndirect",
                                    "glCopyImageSubData",
                                    "glFramebufferParameteri",
                                    "glGetFramebufferParameteriv",
                                    "glGetInternalformati64v",
                                    "glInvalidateTexSubImage",
                                    "glInvalidateTexImage",
                                    "glInvalidateBufferSubData",
                                    "glInvalidateBufferData",
                                    "glInvalidateFramebuffer",
                                    "glInvalidateSubFramebuffer",
                                    "glMultiDrawArraysIndirect",
                                    "glMultiDrawElementsIndirect",
                                    "glGetProgramInterfaceiv",
                                    "glGetProgramResourceIndex",
                                    "glGetProgramResourceName",
                                    "glGetProgramResourceiv",
                                    "glGetProgramResourceLocation",
                                    "glGetProgramResourceLocationIndex",
                                    "glShaderStorageBlockBinding",
                                    "glTexBufferRange",
                                    "glTexStorage2DMultisample",
                                    "glTexStorage3DMultisample",
                                    "glTextureView",
                                    "glBindVertexBuffer",
                                    "glVertexAttribFormat",
                                    "glVertexAttribIFormat",
                                    "glVertexAttribLFormat",
                                    "glVertexAttribBinding",
                                    "glVertexBindingDivisor",
                                    "glDebugMessageControl",
                                    "glDebugMessageInsert",
                                    "glDebugMessageCallback",
                                    "glGetDebugMessageLog",
                                    "glPushDebugGroup",
                                    "glPopDebugGroup",
                                    "glObjectLabel",
                                    "glGetObjectLabel",
                                    "glObjectPtrLabel",
                                    "glGetObjectPtrLabel",
                                    "glBufferStorage",
                                    "glClearTexImage",
                                    "glClearTexSubImage",
                                    "glBindBuffersBase",
                                    "glBindBuffersRange",
                                    "glBindTextures",
                                    "glBindSamplers",
                                    "glBindImageTextures",
                                    "glBindVertexBuffers",
                                    "glClipControl",
                                    "glCreateTransformFeedbacks",
                                    "glTransformFeedbackBufferBase",
                                    "glTransformFeedbackBufferRange",
                                    "glGetTransformFeedbackiv",
                                    "glGetTransformFeedbacki_v",
                                    "glGetTransformFeedbacki64_v",
                                    "glCreateBuffers",
                                    "glNamedBufferStorage",
                                    "glNamedBufferData",
                                    "glNamedBufferSubData",
                                    "glCopyNamedBufferSubData",
                                    "glClearNamedBufferData",
                                    "glClearNamedBufferSubData",
                                    "glMapNamedBuffer",
                                    "glMapNamedBufferRange",
                                    "glUnmapNamedBuffer",
                                    "glFlushMappedNamedBufferRange",
                                    "glGetNamedBufferParameteriv",
                                    "glGetNamedBufferParameteri64v",
                                    "glGetNamedBufferPointerv",
                                    "glGetNamedBufferSubData",
                                    "glCreateFramebuffers",
                                    "glNamedFramebufferRenderbuffer",
                                    "glNamedFramebufferParameteri",
                                    "glNamedFramebufferTexture",
                                    "glNamedFramebufferTextureLayer",
                                    "glNamedFramebufferDrawBuffer",
                                    "glNamedFramebufferDrawBuffers",
                                    "glNamedFramebufferReadBuffer",
                                    "glInvalidateNamedFramebufferData",
                                    "glInvalidateNamedFramebufferSubData",
                                    "glClearNamedFramebufferiv",
                                    "glClearNamedFramebufferuiv",
                                    "glClearNamedFramebufferfv",
                                    "glClearNamedFramebufferfi",
                                    "glBlitNamedFramebuffer",
                                    "glCheckNamedFramebufferStatus",
                                    "glGetNamedFramebufferParameteriv",
                                    "glGetNamedFramebufferAttachmentParameteriv",
                                    "glCreateRenderbuffers",
                                    "glNamedRenderbufferStorage",
                                    "glNamedRenderbufferStorageMultisample",
                                    "glGetNamedRenderbufferParameteriv",
                                    "glCreateTextures",
                                    "glTextureBuffer",
                                    "glTextureBufferRange",
                                    "glTextureStorage1D",
                                    "glTextureStorage2D",
                                    "glTextureStorage3D",
                                    "glTextureStorage2DMultisample",
                                    "glTextureStorage3DMultisample",
                                    "glTextureSubImage1D",
                                    "glTextureSubImage2D",
                                    "glTextureSubImage3D",
                                    "glCompressedTextureSubImage1D",
                                    "glCompressedTextureSubImage2D",
                                    "glCompressedTextureSubImage3D",
                                    "glCopyTextureSubImage1D",
                                    "glCopyTextureSubImage2D",
                                    "glCopyTextureSubImage3D",
                                    "glTextureParameterf",
                                    "glTextureParameterfv",
                                    "glTextureParameteri",
                                    "glTextureParameterIiv",
                                    "glTextureParameterIuiv",
                                    "glTextureParameteriv",
                                    "glGenerateTextureMipmap",
                                    "glBindTextureUnit",
                                    "glGetTextureImage",
                                    "glGetCompressedTextureImage",
                                    "glGetTextureLevelParameterfv",
                                    "glGetTextureLevelParameteriv",
                                    "glGetTextureParameterfv",
                                    "glGetTextureParameterIiv",
                                    "glGetTextureParameterIuiv",
                                    "glGetTextureParameteriv",
                                    "glCreateVertexArrays",
                                    "glDisableVertexArrayAttrib",
                                    "glEnableVertexArrayAttrib",
                                    "glVertexArrayElementBuffer",
                                    "glVertexArrayVertexBuffer",
                                    "glVertexArrayVertexBuffers",
                                    "glVertexArrayAttribBinding",
                                    "glVertexArrayAttribFormat",
                                    "glVertexArrayAttribIFormat",
                                    "glVertexArrayAttribLFormat",
                                    "glVertexArrayBindingDivisor",
                                    "glGetVertexArrayiv",
                                    "glGetVertexArrayIndexediv",
                                    "glGetVertexArrayIndexed64iv",
                                    "glCreateSamplers",
                                    "glCreateProgramPipelines",
                                    "glCreateQueries",
                                    "glGetQueryBufferObjecti64v",
                                    "glGetQueryBufferObjectiv",
                                    "glGetQueryBufferObjectui64v",
                                    "glGetQueryBufferObjectuiv",
                                    "glMemoryBarrierByRegion",
                                    "glGetTextureSubImage",
                                    "glGetCompressedTextureSubImage",
                                    "glGetGraphicsResetStatus",
                                    "glGetnCompressedTexImage",
                                    "glGetnTexImage",
                                    "glGetnUniformdv",
                                    "glGetnUniformfv",
                                    "glGetnUniformiv",
                                    "glGetnUniformuiv",
                                    "glReadnPixels",
                                    "glTextureBarrier",
                                    "glSpecializeShader",
                                    "glMultiDrawArraysIndirectCount",
                                    "glMultiDrawElementsIndirectCount",
                                    "glPolygonOffsetClamp",
                                    nullptr};

inline void clean_glsl_name(char* name, int& name_len)
{
  if(name_len && name[name_len - 1] == ']')
  {
    name_len -= 1;
    while(name_len && name[name_len] != '[')
    {
      name_len -= 1;
    }
  }
  name[name_len] = 0;
}
