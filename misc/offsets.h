#pragma once
#include <windows.h>
#include <cstdint>
#include <string>

namespace Offsets {
    inline std::string ClientVersion = "version-df7528517c6849f7";

    namespace AnimationTrack {
        inline constexpr uintptr_t Animation = 0xd0;
        inline constexpr uintptr_t Animator = 0x118;
        inline constexpr uintptr_t IsPlaying = 0x5a8;
        inline constexpr uintptr_t Looped = 0xf5;
        inline constexpr uintptr_t Speed = 0xe4;
    }

    namespace Animator {
        inline constexpr uintptr_t ActiveAnimationTracks = 0x648;
    }

    namespace Atmosphere {
        inline constexpr uintptr_t Color = 0xd0;
        inline constexpr uintptr_t Decay = 0xdc;
        inline constexpr uintptr_t Density = 0xe8;
        inline constexpr uintptr_t Glare = 0xec;
        inline constexpr uintptr_t Haze = 0xf0;
        inline constexpr uintptr_t Offset = 0xf4;
    }

    namespace Attachment {
        inline constexpr uintptr_t Position = 0xdc;
    }

    namespace BasePart {
        inline constexpr uintptr_t CastShadow = 0xf5;
        inline constexpr uintptr_t Color3 = 0x194;
        inline constexpr uintptr_t Locked = 0xf6;
        inline constexpr uintptr_t Massless = 0xf7;
        inline constexpr uintptr_t Primitive = 0x148;
        inline constexpr uintptr_t Reflectance = 0xec;
        inline constexpr uintptr_t Shape = 0x1b1;
        inline constexpr uintptr_t Transparency = 0xf0;
    }

    namespace BloomEffect {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Intensity = 0xd0;
        inline constexpr uintptr_t Size = 0xd4;
        inline constexpr uintptr_t Threshold = 0xd8;
    }

    namespace ByteCode {
        inline constexpr uintptr_t Pointer = 0x10;
        inline constexpr uintptr_t Size = 0x20;
    }

    namespace Camera {
        inline constexpr uintptr_t CameraMode = 0x158;
        inline constexpr uintptr_t CameraSubject = 0xe8;
        inline constexpr uintptr_t CameraType = 0x0; // unknown
        inline constexpr uintptr_t CFrame = 0xf8;
        inline constexpr uintptr_t FieldOfView = 0x160;
        inline constexpr uintptr_t Orientation = 0xf8;
        inline constexpr uintptr_t Position = 0x11c;
        inline constexpr uintptr_t Rotation = 0xf8;
        inline constexpr uintptr_t ReadOnlyViewportSize = 0x2e8; // vector2 (float)
        inline constexpr uintptr_t ViewportSize = 0x2ac;         // vector2 (int16)
    }

    namespace CharacterMesh {
        inline constexpr uintptr_t BaseTextureId = 0xd0;
        inline constexpr uintptr_t BodyPart = 0xe8;
        inline constexpr uintptr_t MeshId = 0xd8;
        inline constexpr uintptr_t OverlayTextureId = 0xe0;
    }

    namespace ClickDetector {
        inline constexpr uintptr_t MaxActivationDistance = 0x100;
        inline constexpr uintptr_t MouseIcon = 0xe0;
    }

    namespace ColorCorrectionEffect {
        inline constexpr uintptr_t Brightness = 0xdc;
        inline constexpr uintptr_t Contrast = 0xe0;
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Saturation = 0xe4;
        inline constexpr uintptr_t TintColor = 0xd0;
    }

    namespace DataModel {
        inline constexpr uintptr_t ClientReplicator = 0x3d8;
        inline constexpr uintptr_t CreatorId = 0x188;
        inline constexpr uintptr_t GameId = 0x190;
        inline constexpr uintptr_t GameLoaded = 0x5f8;
        inline constexpr uintptr_t JobId = 0x138;
        inline constexpr uintptr_t PlaceId = 0x198;
        inline constexpr uintptr_t PlaceVersion = 0x1b4;
        inline constexpr uintptr_t PrimitiveCount = 0x438;
        inline constexpr uintptr_t RunService = 0x390;
        inline constexpr uintptr_t ScriptContext = 0x3f0;
        inline constexpr uintptr_t ServerIP = 0x5e0;
        inline constexpr uintptr_t UserInputService = 0x3a0;
        inline constexpr uintptr_t Workspace = 0x178;
    }

    namespace DepthOfFieldEffect {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t FarIntensity = 0xd0;
        inline constexpr uintptr_t FocusDistance = 0xd4;
        inline constexpr uintptr_t InFocusRadius = 0xd8;
        inline constexpr uintptr_t NearIntensity = 0xdc;
    }

    namespace FakeDataModel {
        inline constexpr uintptr_t Pointer = 0x7e35858;
        inline constexpr uintptr_t RealDataModel = 0x1c0;
    }

    namespace GuiBase2D {
        inline constexpr uintptr_t AbsolutePosition = 0x10c;
        inline constexpr uintptr_t AbsoluteRotation = 0x188;
        inline constexpr uintptr_t AbsoluteSize = 0x118;
    }

    namespace GuiObject {
        inline constexpr uintptr_t Active = 0x5ac;
        inline constexpr uintptr_t AnchorPoint = 0x560;
        inline constexpr uintptr_t AutomaticSize = 0x568;
        inline constexpr uintptr_t BackgroundColor3 = 0x548;
        inline constexpr uintptr_t BackgroundTransparency = 0x56c;
        inline constexpr uintptr_t BorderColor3 = 0x554;
        inline constexpr uintptr_t BorderMode = 0x570;
        inline constexpr uintptr_t BorderSizePixel = 0x574;
        inline constexpr uintptr_t Text = 0xe08;
        inline constexpr uintptr_t ScreenGui_Enabled = 0x4cc;
        inline constexpr uintptr_t Image = 0xa00;
        inline constexpr uintptr_t RichText = 0xaa8;
        inline constexpr uintptr_t ClipsDescendants = 0x5ad;
        inline constexpr uintptr_t GuiState = 0x580;
        inline constexpr uintptr_t Interactable = 0x5af;
        inline constexpr uintptr_t LayoutOrder = 0x584;
        inline constexpr uintptr_t Position = 0x518;
        inline constexpr uintptr_t Rotation = 0x188;
        inline constexpr uintptr_t Selectable = 0x5b0;
        inline constexpr uintptr_t SelectionOrder = 0x5a0;
        inline constexpr uintptr_t Size = 0x538;
        inline constexpr uintptr_t SizeConstraint = 0x5a4;
        inline constexpr uintptr_t Visible = 0x5b1;
        inline constexpr uintptr_t ZIndex = 0x5a8;
    }

    namespace Highlight {
        inline constexpr uintptr_t DepthMode = 0xf8;
        inline constexpr uintptr_t FillColor = 0xe0;
        inline constexpr uintptr_t FillTransparency = 0xfc;
        inline constexpr uintptr_t OutlineColor = 0xec;
        inline constexpr uintptr_t OutlineTransparency = 0xf0;
    }

    namespace Humanoid {
        inline constexpr uintptr_t AutoJumpEnabled = 0x1d8;
        inline constexpr uintptr_t AutoRotate = 0x1d9;
        inline constexpr uintptr_t AutomaticScalingEnabled = 0x1da;
        inline constexpr uintptr_t BreakJointsOnDeath = 0x1db;
        inline constexpr uintptr_t CameraOffset = 0x140;
        inline constexpr uintptr_t DisplayDistanceType = 0x18c;
        inline constexpr uintptr_t EvaluateStateMachine = 0x1dc;
        inline constexpr uintptr_t HumanoidState = 0x8d8;
        inline constexpr uintptr_t HumanoidStateID = 0x20;
        inline constexpr uintptr_t Health = 0x194;
        inline constexpr uintptr_t HealthDisplayDistance = 0x198;
        inline constexpr uintptr_t HealthDisplayType = 0x19c;
        inline constexpr uintptr_t HipHeight = 0x1a0;
        inline constexpr uintptr_t JumpHeight = 0x1ac;
        inline constexpr uintptr_t MoveDirection = 0x158;
        inline constexpr uintptr_t JumpPower = 0x1b0;
        inline constexpr uintptr_t MaxHealth = 0x1b4;
        inline constexpr uintptr_t MaxSlopeAngle = 0x1b8;
        inline constexpr uintptr_t NameDisplayDistance = 0x1bc;
        inline constexpr uintptr_t NameOcclusion = 0x1c0;
        inline constexpr uintptr_t RequiresNeck = 0x1e0;
        inline constexpr uintptr_t RigType = 0x1c8;
        inline constexpr uintptr_t Sit = 0x1e1;
        inline constexpr uintptr_t TargetPoint = 0x164;
        inline constexpr uintptr_t UseJumpPower = 0x1e3;
        inline constexpr uintptr_t WalkSpeed = 0x1d4;
        inline constexpr uintptr_t WalkSpeedCheck = 0x3c0;
        inline constexpr uintptr_t WalkToPoint = 0x17c;
    }

    namespace InputObject {
        inline constexpr uintptr_t MousePosition = 0xec;
    }

    namespace Instance {
        inline constexpr uintptr_t AttributeContainer = 0x48;
        inline constexpr uintptr_t AttributeList = 0x18;
        inline constexpr uintptr_t AttributeToNext = 0x58;
        inline constexpr uintptr_t AttributeToValue = 0x18;
        inline constexpr uintptr_t ChildrenEnd = 0x8;
        inline constexpr uintptr_t ChildrenStart = 0x70;
        inline constexpr uintptr_t ClassBase = 0xcf0;
        inline constexpr uintptr_t ClassDescriptor = 0x18;
        inline constexpr uintptr_t ClassName = 0x8;
        inline constexpr uintptr_t Name = 0xb0;
        inline constexpr uintptr_t Parent = 0x68;
        inline constexpr uintptr_t This = 0x8;
    }

    namespace Lighting {
        inline constexpr uintptr_t Ambient = 0xd8;
        inline constexpr uintptr_t Atmosphere = 0x1e8;
        inline constexpr uintptr_t Brightness = 0x120;
        inline constexpr uintptr_t ClearAlpha = 0x1b0;
        inline constexpr uintptr_t ClearColor = 0x1a4;
        inline constexpr uintptr_t ClockTime = 0x1b8;
        inline constexpr uintptr_t ColorShift_Bottom = 0xe4;
        inline constexpr uintptr_t ColorShift_Top = 0xf0;
        inline constexpr uintptr_t EnvironmentDiffuseScale = 0x124;
        inline constexpr uintptr_t EnvironmentSpecularScale = 0x128;
        inline constexpr uintptr_t ExposureCompensation = 0x12c;
        inline constexpr uintptr_t FogColor = 0xfc;
        inline constexpr uintptr_t FogEnd = 0x134;
        inline constexpr uintptr_t FogStart = 0x138;
        inline constexpr uintptr_t GeographicLatitude = 0x190;
        inline constexpr uintptr_t GlobalShadows = 0x148;
        inline constexpr uintptr_t HasSky = 0x1a0;
        inline constexpr uintptr_t OutdoorAmbient = 0x108;
        inline constexpr uintptr_t ShadowSoftness = 0x140;
        inline constexpr uintptr_t Sky = 0x1d8;
    }

    namespace LocalScript {
        inline constexpr uintptr_t Bytecode = 0x1a8;
        inline constexpr uintptr_t Hash = 0xe8;
    }

    namespace MaterialColors {
        inline constexpr uintptr_t Asphalt = 0x30;
        inline constexpr uintptr_t Basalt = 0x27;
        inline constexpr uintptr_t Brick = 0xf;
        inline constexpr uintptr_t Cobblestone = 0x33;
        inline constexpr uintptr_t Concrete = 0xc;
        inline constexpr uintptr_t CrackedLava = 0x2d;
        inline constexpr uintptr_t Glacier = 0x1b;
        inline constexpr uintptr_t Grass = 0x6;
        inline constexpr uintptr_t Ground = 0x2a;
        inline constexpr uintptr_t Ice = 0x36;
        inline constexpr uintptr_t LeafyGrass = 0x39;
        inline constexpr uintptr_t Limestone = 0x3f;
        inline constexpr uintptr_t Mud = 0x24;
        inline constexpr uintptr_t Pavement = 0x42;
        inline constexpr uintptr_t Rock = 0x18;
        inline constexpr uintptr_t Salt = 0x3c;
        inline constexpr uintptr_t Sand = 0x12;
        inline constexpr uintptr_t Sandstone = 0x21;
        inline constexpr uintptr_t Slate = 0x9;
        inline constexpr uintptr_t Snow = 0x1e;
        inline constexpr uintptr_t WoodPlanks = 0x15;
    }

    namespace MeshPart {
        inline constexpr uintptr_t MeshId = 0x2e8;
        inline constexpr uintptr_t TextureId = 0x318;
    }

    namespace Misc {
        inline constexpr uintptr_t Adornee = 0xd0;
        inline constexpr uintptr_t AnimationId = 0xd0;
        inline constexpr uintptr_t DecalTexture = 0x198;
        inline constexpr uintptr_t Ping = 0xcc;
        inline constexpr uintptr_t RequireLock = 0x0;
        inline constexpr uintptr_t StringLength = 0x10;
    }

    namespace Model {
        inline constexpr uintptr_t PrimaryPart = 0x278;
        inline constexpr uintptr_t Scale = 0x164;
    }

    namespace ModuleScript {
        inline constexpr uintptr_t Bytecode = 0x150;
        inline constexpr uintptr_t Hash = 0x160;
    }

    namespace MouseService {
        inline constexpr uintptr_t InputObject = 0x110;
        inline constexpr uintptr_t SensitivityPointer = 0x7ebd660;
    }

    namespace Player {
        inline constexpr uintptr_t AccountAge = 0x30c;
        inline constexpr uintptr_t CameraMode = 0x318;
        inline constexpr uintptr_t Country = 0x110;
        inline constexpr uintptr_t DisplayName = 0x130;
        inline constexpr uintptr_t Gender = 0xeb8;
        inline constexpr uintptr_t HealthDisplayDistance = 0x338;
        inline constexpr uintptr_t LocaleId = 0x6b8;
        inline constexpr uintptr_t MaxZoomDistance = 0x310;
        inline constexpr uintptr_t MinZoomDistance = 0x314;
        inline constexpr uintptr_t ModelInstance = 0x380;
        inline constexpr uintptr_t Mouse = 0xd28;
        inline constexpr uintptr_t NameDisplayDistance = 0x344;
        inline constexpr uintptr_t Team = 0x290;
        inline constexpr uintptr_t TeamColor = 0x350;
        inline constexpr uintptr_t UserId = 0x2b8;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x130;
    }

    namespace PlayerConfigurer {
        inline constexpr uintptr_t OverrideDuration = 0x3b8;
        inline constexpr uintptr_t Pointer = 0x7e12fd0;
    }

    namespace PlayerMouse {
        inline constexpr uintptr_t Icon = 0xe0;
        inline constexpr uintptr_t Workspace = 0x168;
    }

    namespace Primitive {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0xfc;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xf0;
        inline constexpr uintptr_t CFrame = 0xc0;
        inline constexpr uintptr_t Flags = 0x1ae;
        inline constexpr uintptr_t Material = 0x246;
        inline constexpr uintptr_t Orientation = 0xc0;
        inline constexpr uintptr_t Owner = 0x210;
        inline constexpr uintptr_t Position = 0xe4;
        inline constexpr uintptr_t Rotation = 0xc0;
        inline constexpr uintptr_t Size = 0x1b0;
        inline constexpr uintptr_t Validate = 0x6;
    }

    namespace PrimitiveFlags {
        inline constexpr uintptr_t Anchored = 0x2;
        inline constexpr uintptr_t CanCollide = 0x8;
        inline constexpr uintptr_t CanTouch = 0x10;
    }

    namespace ProximityPrompt {
        inline constexpr uintptr_t ActionText = 0xd0;
        inline constexpr uintptr_t Enabled = 0x156;
        inline constexpr uintptr_t GamepadKeyCode = 0x13c;
        inline constexpr uintptr_t HoldDuration = 0x140;
        inline constexpr uintptr_t KeyboardKeyCode = 0x144;
        inline constexpr uintptr_t KeyCode = 0x144;
        inline constexpr uintptr_t MaxActivationDistance = 0x148;
        inline constexpr uintptr_t ObjectText = 0xf0;
        inline constexpr uintptr_t RequiresLineOfSight = 0x157;
    }

    namespace RenderView {
        inline constexpr uintptr_t DeviceD3D11 = 0x8;
        inline constexpr uintptr_t VisualEngine = 0x10;
        inline constexpr uintptr_t LightingValid = 0x148;
        inline constexpr uintptr_t SkyboxValid = 0x28d;
    }

    namespace RunService {
        inline constexpr uintptr_t HeartbeatFPS = 0xb8;
        inline constexpr uintptr_t HeartbeatTask = 0xe8;
    }

    namespace Sky {
        inline constexpr uintptr_t MoonAngularSize = 0x25c;
        inline constexpr uintptr_t MoonTextureId = 0xe0;
        inline constexpr uintptr_t SkyboxBk = 0x110;
        inline constexpr uintptr_t SkyboxDn = 0x140;
        inline constexpr uintptr_t SkyboxFt = 0x170;
        inline constexpr uintptr_t SkyboxLf = 0x1a0;
        inline constexpr uintptr_t SkyboxOrientation = 0x250;
        inline constexpr uintptr_t SkyboxRt = 0x1d0;
        inline constexpr uintptr_t SkyboxUp = 0x200;
        inline constexpr uintptr_t StarCount = 0x260;
        inline constexpr uintptr_t SunAngularSize = 0x264;
        inline constexpr uintptr_t SunTextureId = 0x230;
    }

    namespace SkyParameters {
        inline constexpr uintptr_t LightColor = 0x15c;
        inline constexpr uintptr_t LightDirection = 0x168;
        inline constexpr uintptr_t MoonPosition = 0x184;
        inline constexpr uintptr_t SkyAmbient = 0x150;
        inline constexpr uintptr_t SkyAmbient2 = 0x194;
        inline constexpr uintptr_t SunPosition = 0x178;
        inline constexpr uintptr_t Source = 0x174;
    }

    namespace SpecialMesh {
        inline constexpr uintptr_t MeshId = 0x108;
        inline constexpr uintptr_t Offset = 0xd0;
        inline constexpr uintptr_t Scale = 0xdc;
        inline constexpr uintptr_t TextureId = 0x130;
    }

    namespace StatsItem {
        inline constexpr uintptr_t Value = 0x1c8;
    }

    namespace SunRaysEffect {
        inline constexpr uintptr_t Enabled = 0xc8;
        inline constexpr uintptr_t Intensity = 0xd0;
        inline constexpr uintptr_t Spread = 0xd4;
    }

    namespace TaskScheduler {
        inline constexpr uintptr_t JobEnd = 0x1d8;
        inline constexpr uintptr_t JobName = 0x18;
        inline constexpr uintptr_t JobStart = 0x1d0;
        inline constexpr uintptr_t MaxFPS = 0x1b0;
        inline constexpr uintptr_t Pointer = 0x7ef3c48;
        inline constexpr uintptr_t RenderJobToFakeDataModel = 0x38;
        inline constexpr uintptr_t RenderJobToRenderView = 0x218;
    }

    namespace Team {
        inline constexpr uintptr_t TeamColor = 0xd0;
        inline constexpr uintptr_t BrickColor = 0xd0;
    }

    namespace Terrain {
        inline constexpr uintptr_t GrassLength = 0x1f8;
        inline constexpr uintptr_t MaterialColors = 0x280;
        inline constexpr uintptr_t WaterColor = 0x1e8;
        inline constexpr uintptr_t WaterReflectance = 0x200;
        inline constexpr uintptr_t WaterTransparency = 0x204;
        inline constexpr uintptr_t WaterWaveSize = 0x208;
        inline constexpr uintptr_t WaterWaveSpeed = 0x20c;
    }

    namespace TextButton {
        inline constexpr uintptr_t AutoButtonColor = 0xa14;
        inline constexpr uintptr_t ContentText = 0xd28;
        inline constexpr uintptr_t Font = 0x1150;
        inline constexpr uintptr_t LineHeight = 0xd9c;
        inline constexpr uintptr_t LocalizedText = 0xd28;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0x115c;
        inline constexpr uintptr_t Modal = 0xa15;
        inline constexpr uintptr_t RichText = 0xe45;
        inline constexpr uintptr_t Selected = 0xa16;
        inline constexpr uintptr_t Text = 0xd28;
        inline constexpr uintptr_t TextColor3 = 0x1138;
        inline constexpr uintptr_t TextDirection = 0xe48;
        inline constexpr uintptr_t TextScaled = 0xd98;
        inline constexpr uintptr_t TextSize = 0x1164;
        inline constexpr uintptr_t TextStrokeColor3 = 0x1144;
        inline constexpr uintptr_t TextStrokeTransparency = 0x1168;
        inline constexpr uintptr_t TextTransparency = 0x116c;
        inline constexpr uintptr_t TextTruncate = 0x1170;
        inline constexpr uintptr_t TextWrapped = 0xde8;
        inline constexpr uintptr_t TextXAlignment = 0x1174;
        inline constexpr uintptr_t TextYAlignment = 0xde4;
        inline constexpr uintptr_t TextBounds = 0xf80;
    }

    namespace TextLabel {
        inline constexpr uintptr_t ContentText = 0xaa8;
        inline constexpr uintptr_t Font = 0xed0;
        inline constexpr uintptr_t LineHeight = 0xb1c;
        inline constexpr uintptr_t LocalizedText = 0xaa8;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0xedc;
        inline constexpr uintptr_t RichText = 0xbc5;
        inline constexpr uintptr_t Text = 0xaa8;
        inline constexpr uintptr_t TextColor3 = 0xeb8;
        inline constexpr uintptr_t TextDirection = 0xbc8;
        inline constexpr uintptr_t TextScaled = 0xb18;
        inline constexpr uintptr_t TextSize = 0xee4;
        inline constexpr uintptr_t TextStrokeColor3 = 0xec4;
        inline constexpr uintptr_t TextStrokeTransparency = 0xee8;
        inline constexpr uintptr_t TextTransparency = 0xeec;
        inline constexpr uintptr_t TextTruncate = 0xef0;
        inline constexpr uintptr_t TextWrapped = 0xb68;
        inline constexpr uintptr_t TextXAlignment = 0xef4;
        inline constexpr uintptr_t TextYAlignment = 0xb64;
        inline constexpr uintptr_t TextBounds = 0xd00;
    }

    namespace TextBox {
        inline constexpr uintptr_t ContentText = 0xac0;
        inline constexpr uintptr_t LineHeight = 0xb34;
        inline constexpr uintptr_t Text = 0xe20;
        inline constexpr uintptr_t TextBounds = 0xcf8;
        inline constexpr uintptr_t TextColor3 = 0xefc;
        inline constexpr uintptr_t TextSize = 0xcfc;
        inline constexpr uintptr_t TextStrokeColor3 = 0xf08;
        inline constexpr uintptr_t TextTransparency = 0xf40;
    }

    namespace Textures {
        inline constexpr uintptr_t Decal_Texture = 0x198;
        inline constexpr uintptr_t Texture_Texture = 0x198;
    }

    namespace Tool {
        inline constexpr uintptr_t CanBeDropped = 0x4a0;
        inline constexpr uintptr_t Enabled = 0x4a1;
        inline constexpr uintptr_t Grip = 0x470;
        inline constexpr uintptr_t GripForward = 0x488;
        inline constexpr uintptr_t GripPos = 0x494;
        inline constexpr uintptr_t GripRight = 0x470;
        inline constexpr uintptr_t GripUp = 0x47c;
        inline constexpr uintptr_t ManualActivationOnly = 0x4a2;
        inline constexpr uintptr_t RequiresHandle = 0x4a3;
        inline constexpr uintptr_t Tooltip = 0x450;
    }

    namespace UIGradient {
        inline constexpr uintptr_t Color = 0x134;
        inline constexpr uintptr_t Offset = 0x158;
        inline constexpr uintptr_t Rotation = 0x160;
        inline constexpr uintptr_t Transparency = 0xf4;
    }

    namespace ValueObject {
        inline constexpr uintptr_t Value = 0xd0;
    }

    namespace VisualEngine {
        inline constexpr uintptr_t Dimensions = 0x720;
        inline constexpr uintptr_t FakeDataModel = 0x700;
        inline constexpr uintptr_t Pointer = 0x79e9468;
        inline constexpr uintptr_t RenderView = 0x800;
        inline constexpr uintptr_t ViewMatrix = 0x120;
    }

    namespace Workspace {
        inline constexpr uintptr_t CurrentCamera = 0x4a0;
        inline constexpr uintptr_t DistributedGameTime = 0x4c0;
        inline constexpr uintptr_t ReadOnlyGravity = 0xa28;
        inline constexpr uintptr_t World = 0x48;
        inline constexpr uintptr_t World2 = 0x3d8;
        inline constexpr uintptr_t World3 = 0x948;
    }

    namespace World {
        inline constexpr uintptr_t Gravity = 0x1d0;
        inline constexpr uintptr_t PrimitivesList = 0x240;

        namespace SteppingParameters {
            inline constexpr uintptr_t m_worldStepsPerSec = 0x658;
        }
    }
}