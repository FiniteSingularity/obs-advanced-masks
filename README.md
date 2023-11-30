<!-- Title -->
# OBS Advanced Masks

<p align="center">
    <i align="center">Enhance your OBS Studio masking capabilities with advanced mask options.</i>
</p>

<h4 align="center">
    <a href="https://github.com/FiniteSingularity/obs-advanced-masks/releases">
        <img src="https://img.shields.io/github/v/release/finitesingularity/obs-advanced-masks?filter=*&style=flat-square&label=Latest" alt="Latest release">
    </a>
    <img src="https://img.shields.io/badge/OBS-28_|_29_|_30-blue.svg?style=flat-square" alt="supports obs versions 28, 29, and 30">
    <img src="https://img.shields.io/badge/Windows-0078D6?style=flat-square&logo=windows&logoColor=white">
    <img src="https://img.shields.io/badge/mac%20os-000000?style=flat-square&logo=apple&logoColor=white">
    <img src="https://img.shields.io/badge/Linux-FCC624?style=flat-square&logo=linux&logoColor=black"><br>
    <a href="https://twitter.com/FiniteSingulrty">
        <img src="https://img.shields.io/badge/Twitter-1DA1F2?style=flat-square&logo=twitter&logoColor=white">
    </a>
    <a href="https://twitch.tv/finitesingularity">
        <img src="https://img.shields.io/badge/Twitch-9146FF?style=flat-square&logo=twitch&logoColor=white">
    </a>
</h4>

> **Note**
> While we only release new versions of Advanced Mask after testing it on local hardware, there are bugs and issues that will slip through. If you happen to run into any issues, please [open an issue](https://github.com/finitesingularity/obs-advanced-masks/issues) and we will work to resolve it.

## Introduction

OBS Advanced Masks is an open-source project designed to expand the masking functionalities within OBS Studio. This plug-in provides filters for users to create intricate and customized masks for their OBS Scenes and Sources.

- 🎭 Advanced Masks provides both [Alpha Masking](#alpha-masking) and [Adjustment Masking](#adjustment-masking) and supports Windows, MacOS, and Linux.
- 🟠 Shape masks allow for dynamically generated [Rectangle](#rectangle), [Circle](#circle), [Elliptical](#elliptical), [Regular Polygon](#regular-polygon), [Star](#star), and [Heart](#heart) shaped masks, with many adjustable parameters.
- 🎥 Source masks allow an existing OBS source to be used as a mask, using any combination of the red, green, blue, or alpha channels from said source.
- 🖼️ Image masks include all of the same functionality as Source Masks, but applied via a static image (.png, .jpeg, etc).
- 🌈 Gradient masks allow a fading mask using a user-specified gradient.

## Mask Types
Advanced Masks provides both [Alpha Masking](#alpha-masking) and [Adjustment Masking](#adjustment-masking) and supports Windows, MacOS, and Linux.

### Alpha Masking
An alpha mask, when applied to an OBS source, serves as a mechanism to control the transparency or opacity of specific areas within the source. It essentially defines which parts of the source are visible, partially visible, or completely transparent.

### Adjustment Masking
A color adjustment mask modifies specific colors or color ranges within an OBS source, allowing targeted changes to the hue, saturation, brightness, and contrast. It enables precise adjustments to selected areas, providing control over the visual appearance without affecting the entire source uniformly.

## Mask Algorithms
Intro to the different mask algorithms.

### Shape Masks
Shape masks allow you to specify a parameterized shape which defines the masked area.

Something about canvas placement and Scene View Transformation option.

Something about ability to zoom source when masking with a shape.

#### Rectangle
The rectangular mask is a customizable mask used to frame an OBS Source with a rectangular shape featuring rounded corners. This mask offers the flexibility to adjust the width, height, rotation, and, corner radius, allowing users to modify the curvature of each corner independently. Users can optionally set and modify the corner radius, enabling them to create rectangular shapes with varying degrees of roundedness, from sharp edges to completely rounded corners, based on their preferences or design requirements.

#### Circle
The circular mask is used to frame an OBS Source within a perfect circle. This mask allows users to specify the desired radius, enabling them to create circular masks of varying sizes, from small circles to larger ones, offering flexibility in how the content within the circle is displayed or highlighted.

#### Elliptical
The ellipse mask is used to frame an OBS Source within an ellipse, offering flexibility in its dimensions and orientation. Users can define the width and height of the ellipse independently, allowing for an elliptical shape that can be taller, wider, or more elongated based on their specifications. Additionally, users have the option to adjust the rotation of the ellipse, enabling them to orient it at various angles within the scene. This level of control allows for the creation of elliptical masks of different proportions and orientations to suit specific design or visual requirements.

#### Regular Polygon
The regular polygon mask allows users to frame an OBS source within a polygonal shape with distinct characteristics. Users can specify:

1. Number of Sides: Determines the polygon's shape, allowing for triangles, squares, pentagons, hexagons, and more, based on the chosen number of sides.
1. Polygon Radius: Defines the size or extent of the polygon, determining how large or small the shape appears within the Scene.
1. Corner Radius: Adds the option to round off the polygon's corners, softening the edges and creating a smoother appearance.
1. Rotation: Enables users to orient the polygon at different angles within the OBS scene, offering diverse orientations for creative framing.

This level of customization provides users with the ability to create regular polygonal masks of various shapes, sizes, corner styles, and orientations, allowing for versatile geometric framing tailored to specific visual preferences or design requirements.

#### Star

The star mask offers users a versatile tool for framing OBS Sources within a star-shaped pattern, with the ability to specify various parameters:

1. Number of Spikes: Allows users to define the number of points or spikes on the star, determining its overall shape, such as five-pointed, six-pointed, or multi-pointed stars.
2. Inner and Outer Radius: Enables users to set both the inner and outer radii of the star, controlling the size and spike length of the star shape.
3. Corner Radius: Provides an option to adjust the corner radius, allowing users to round off the corners of the star's spikes, creating a smoother and softer appearance. Even allowing for non-star shapes such as flowers.
4. Orientation: Allows users to rotate the star mask to different angles within the image or video frame, providing flexibility in positioning the star shape.

This comprehensive set of customizable parameters empowers users to create star-shaped masks with varying numbers of spikes, sizes, corner styles, and orientations, offering flexibility for creative framing and visual effects according to specific preferences or design requirements.

#### Heart
The heart-shaped mask grants users the ability to encase OBS sources within the iconic outline of a heart. Users can precisely define two fundamental parameters: size and orientation. The size parameter allows users to scale the heart shape, making it larger or smaller within the OBS Scene. Meanwhile, the orientation feature empowers users to rotate the heart mask, providing the flexibility to position the heart shape at diverse angles. These adjustable parameters facilitate the creation of heart-shaped masks in various sizes and orientations, presenting a recognizable and customizable visual element for framing content with the symbolic and universally understood shape of a heart. *Special thanks to GPT for this fantastically over the top description of a heart mask*


Thank you for exploring OBS Advanced Masks! Your feedback and contributions are highly appreciated as we continue to improve and expand this project.
