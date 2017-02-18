//
//  builtin_texture.cpp
//
//  Created by 渡部 心 on 2016/08/20.
//  Copyright © 2016年 渡部 心. All rights reserved.
//

#include "builtin_texture.hpp"
#include "../../textures.hpp"

namespace SLRSceneGraph {
    namespace BuiltinFunctions {
        namespace Texture {
            const Element Texture2DMapping = Element::create<TypeMap::Function>(1,
                                                                                std::vector<ArgInfo>{
                                                                                    {"type", Type::String, Element::create<TypeMap::String>("texcoord 2D")}, {"params", Type::Tuple, Element::create<TypeMap::Tuple>()}
                                                                                },
                                                                                [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                    std::string type = args.at("type").raw<TypeMap::String>();
                                                                                    const ParameterList &params = args.at("params").raw<TypeMap::Tuple>();
                                                                                    (void)params;
                                                                                    if (type == "texcoord 2D") {
                                                                                        Texture2DMappingRef rawRef = Texture2DMapping::sharedInstanceRef();
                                                                                        return Element::createFromReference<TypeMap::Texture2DMapping>(rawRef);
                                                                                    }
                                                                                    *err = ErrorMessage("Specified type is invalid.");
                                                                                    return Element();
                                                                                });
            const Element Texture3DMapping = Element::create<TypeMap::Function>(1,
                                                                                std::vector<ArgInfo>{
                                                                                    {"type", Type::String, Element::create<TypeMap::String>("texcoord 2D")}, {"params", Type::Tuple, Element::create<TypeMap::Tuple>()}
                                                                                },
                                                                                [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                    std::string type = args.at("type").raw<TypeMap::String>();
                                                                                    const ParameterList &params = args.at("params").raw<TypeMap::Tuple>();
                                                                                    (void)params;
                                                                                    if (type == "texcoord 2D") {
                                                                                        Texture3DMappingRef rawRef = Texture3DMapping::sharedInstanceRef();
                                                                                        return Element::createFromReference<TypeMap::Texture3DMapping>(rawRef);
                                                                                    }
                                                                                    else if (type == "world pos") {
                                                                                        Texture3DMappingRef rawRef = WorldPosition3DMapping::sharedInstanceRef();
                                                                                        return Element::createFromReference<TypeMap::Texture3DMapping>(rawRef);
                                                                                    }
                                                                                    *err = ErrorMessage("Specified type is invalid.");
                                                                                    return Element();
                                                                                });
            const Element SpectrumTexture = Element::create<TypeMap::Function>(1,
                                                                               std::vector<std::vector<ArgInfo>>{
                                                                                   {{"spectrum", Type::Spectrum}},
                                                                                   {{"image", Type::Image2D}, {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}},
                                                                                   {{"procedure", Type::String}, {"params", Type::Tuple}}
                                                                               },
                                                                               std::vector<Function::Procedure>{
                                                                                   [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                       AssetSpectrumRef spectrum = args.at("spectrum").rawRef<TypeMap::Spectrum>();
                                                                                       SpectrumTextureRef rawRef = createShared<ConstantSpectrumTexture>(spectrum);
                                                                                       return Element::createFromReference<TypeMap::SpectrumTexture>(rawRef);
                                                                                   },
                                                                                   [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                       const auto &image = args.at("image").rawRef<TypeMap::Image2D>();
                                                                                       const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                       SpectrumTextureRef rawRef = createShared<ImageSpectrumTexture>(mapping, image);
                                                                                       return Element::createFromReference<TypeMap::SpectrumTexture>(rawRef);
                                                                                   },
                                                                                   [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                       std::string procedure = args.at("procedure").raw<TypeMap::String>();
                                                                                       const ParameterList &params = args.at("params").raw<TypeMap::Tuple>();
                                                                                       if (procedure == "checker board") {
                                                                                           const static Function configFunc{
                                                                                               0, {
                                                                                                   {"c0", Type::Spectrum}, {"c1", Type::Spectrum}, {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}
                                                                                               },
                                                                                               [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                   const AssetSpectrumRef c0 = args.at("c0").rawRef<TypeMap::Spectrum>();
                                                                                                   const AssetSpectrumRef c1 = args.at("c1").rawRef<TypeMap::Spectrum>();
                                                                                                   const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                                   SpectrumTextureRef rawRef = createShared<CheckerBoardSpectrumTexture>(mapping, c0, c1);
                                                                                                   return Element::createFromReference<TypeMap::SpectrumTexture>(rawRef);
                                                                                               }
                                                                                           };
                                                                                           return configFunc(params, context, err);
                                                                                       }
                                                                                       else if (procedure == "voronoi") {
                                                                                           const static Function configFunc{
                                                                                               0, {
                                                                                                   {"scale", Type::RealNumber}, {"brightness", Type::RealNumber, Element::create<TypeMap::RealNumber>(0.8f)}, {"mapping", Type::Texture3DMapping, Element::createFromReference<TypeMap::Texture3DMapping>(WorldPosition3DMapping::sharedInstanceRef())}
                                                                                               },
                                                                                               [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                   float scale = args.at("scale").raw<TypeMap::RealNumber>();
                                                                                                   float brightness = args.at("brightness").raw<TypeMap::RealNumber>();
                                                                                                   const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture3DMapping>();
                                                                                                   SpectrumTextureRef rawRef = createShared<VoronoiSpectrumTexture>(mapping, scale, brightness);
                                                                                                   return Element::createFromReference<TypeMap::SpectrumTexture>(rawRef);
                                                                                               }
                                                                                           };
                                                                                           return configFunc(params, context, err);
                                                                                       }
                                                                                       *err = ErrorMessage("Specified procedure is invalid.");
                                                                                       return Element();
                                                                                   }
                                                                               });
            const Element NormalTexture = Element::create<TypeMap::Function>(1,
                                                                             std::vector<std::vector<ArgInfo>>{
                                                                                 {{"image", Type::Image2D}, {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}},
                                                                                 {{"procedure", Type::String}, {"params", Type::Tuple}}
                                                                             },
                                                                             std::vector<Function::Procedure>{
                                                                                 [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                     const auto &image = args.at("image").rawRef<TypeMap::Image2D>();
                                                                                     const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                     NormalTextureRef rawRef = createShared<ImageNormalTexture>(mapping, image);
                                                                                     return Element::createFromReference<TypeMap::NormalTexture>(rawRef);
                                                                                 },
                                                                                 [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                     std::string procedure = args.at("procedure").raw<TypeMap::String>();
                                                                                     const ParameterList &params = args.at("params").raw<TypeMap::Tuple>();
                                                                                     if (procedure == "checker board") {
                                                                                         const static Function configFunc{
                                                                                             0, {
                                                                                                 {"stepWidth", Type::RealNumber, Element(0.05)},
                                                                                                 {"reverse", Type::Bool, Element(false)},
                                                                                                 {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}
                                                                                             },
                                                                                             [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                 float stepWidth = args.at("stepWidth").raw<TypeMap::RealNumber>();
                                                                                                 bool reverse = args.at("reverse").raw<TypeMap::Bool>();
                                                                                                 const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                                 NormalTextureRef rawRef = createShared<CheckerBoardNormalTexture>(mapping, stepWidth, reverse);
                                                                                                 return Element::createFromReference<TypeMap::NormalTexture>(rawRef);
                                                                                             }
                                                                                         };
                                                                                         return configFunc(params, context, err);
                                                                                     }
                                                                                     else if (procedure == "voronoi") {
                                                                                         const static Function configFunc{
                                                                                             0, {
                                                                                                 {"scale", Type::RealNumber},
                                                                                                 {"thetaMax", Type::RealNumber, Element(M_PI / 6)},
                                                                                                 {"mapping", Type::Texture3DMapping, Element::createFromReference<TypeMap::Texture3DMapping>(WorldPosition3DMapping::sharedInstanceRef())}
                                                                                             },
                                                                                             [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                 float scale = args.at("scale").raw<TypeMap::RealNumber>();
                                                                                                 float thetaMax = args.at("thetaMax").raw<TypeMap::RealNumber>();
                                                                                                 const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture3DMapping>();
                                                                                                 NormalTextureRef rawRef = createShared<VoronoiNormalTexture>(mapping, scale, thetaMax);
                                                                                                 return Element::createFromReference<TypeMap::NormalTexture>(rawRef);
                                                                                             }
                                                                                         };
                                                                                         return configFunc(params, context, err);
                                                                                     }
                                                                                     *err = ErrorMessage("Specified procedure is invalid.");
                                                                                     return Element();
                                                                                 }
                                                                             });
            const Element FloatTexture = Element::create<TypeMap::Function>(1,
                                                                            std::vector<std::vector<ArgInfo>>{
                                                                                {{"value", Type::RealNumber}},
                                                                                {{"image", Type::Image2D}, {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}},
                                                                                {{"procedure", Type::String}, {"params", Type::Tuple}}
                                                                            },
                                                                            std::vector<Function::Procedure>{
                                                                                [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                    auto value = args.at("value").raw<TypeMap::RealNumber>();
                                                                                    FloatTextureRef rawRef = createShared<ConstantFloatTexture>(value);
                                                                                    return Element::createFromReference<TypeMap::FloatTexture>(rawRef);
                                                                                },
                                                                                [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                    const auto &image = args.at("image").rawRef<TypeMap::Image2D>();
                                                                                    const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                    FloatTextureRef rawRef = createShared<ImageFloatTexture>(mapping, image);
                                                                                    return Element::createFromReference<TypeMap::FloatTexture>(rawRef);
                                                                                },
                                                                                [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                    std::string procedure = args.at("procedure").raw<TypeMap::String>();
                                                                                    const ParameterList &params = args.at("params").raw<TypeMap::Tuple>();
                                                                                    if (procedure == "checker board") {
                                                                                        const static Function configFunc{
                                                                                            0, {
                                                                                                {"c0", Type::RealNumber}, {"c1", Type::RealNumber}, {"mapping", Type::Texture2DMapping, Element::createFromReference<TypeMap::Texture2DMapping>(Texture2DMapping::sharedInstanceRef())}
                                                                                            },
                                                                                            [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                float c0 = args.at("c0").raw<TypeMap::RealNumber>();
                                                                                                float c1 = args.at("c1").raw<TypeMap::RealNumber>();
                                                                                                const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture2DMapping>();
                                                                                                FloatTextureRef rawRef = createShared<CheckerBoardFloatTexture>(mapping, c0, c1);
                                                                                                return Element::createFromReference<TypeMap::FloatTexture>(rawRef);
                                                                                            }
                                                                                        };
                                                                                        return configFunc(params, context, err);
                                                                                    }
                                                                                    else if (procedure == "voronoi") {
                                                                                        const static Function configFunc{
                                                                                            0, {
                                                                                                {"scale", Type::RealNumber},
                                                                                                {"valueScale", Type::RealNumber, Element(1.0)},
                                                                                                {"flat", Type::Bool, Element(true)},
                                                                                                {"mapping", Type::Texture3DMapping, Element::createFromReference<TypeMap::Texture3DMapping>(WorldPosition3DMapping::sharedInstanceRef())}
                                                                                            },
                                                                                            [](const std::map<std::string, Element> &args, ExecuteContext &context, ErrorMessage* err) {
                                                                                                float scale = args.at("scale").raw<TypeMap::RealNumber>();
                                                                                                float valueScale = args.at("valueScale").raw<TypeMap::RealNumber>();
                                                                                                bool flat = args.at("flat").raw<TypeMap::Bool>();
                                                                                                const auto &mapping = args.at("mapping").rawRef<TypeMap::Texture3DMapping>();
                                                                                                FloatTextureRef rawRef = createShared<VoronoiFloatTexture>(mapping, scale, valueScale, flat);
                                                                                                return Element::createFromReference<TypeMap::FloatTexture>(rawRef);
                                                                                            }
                                                                                        };
                                                                                        return configFunc(params, context, err);
                                                                                    }
                                                                                    *err = ErrorMessage("Specified procedure is invalid.");
                                                                                    return Element();
                                                                                }
                                                                            });
        }
    }
}
