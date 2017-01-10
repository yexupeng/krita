/*
 *  Copyright (c) 2016 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef LIBKIS_NODE_H
#define LIBKIS_NODE_H

#include <QObject>

#include <kis_types.h>

#include "kritalibkis_export.h"
#include "libkis.h"

/**
 * Node
 */
class KRITALIBKIS_EXPORT Node : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Node)

public:
    explicit Node(KisImageSP image, KisNodeSP node, QObject *parent = 0);
    virtual ~Node();

    /**
     * @brief alphaLocked checks whether the node is a paint layer and returns whether it is alpha locked
     * @return whether the paint layer is alpha locked, or false if the node is not a paint layer
     */
    bool alphaLocked() const;

    /**
     * @brief setAlphaLocked set the layer to value if the the node is paint layer.
     */
    void setAlphaLocked(bool value);

    /**
     * @return the blending mode of the layer. The values of the blending modes are defined in @see KoCompositeOpRegistry.h
     */
    QString blendingMode() const;

    /**
     * @brief setBlendingMode set the blending mode of the node to the given value
     * @param value one of the string values from @see KoCompositeOpRegistry.h
     */
    void setBlendingMode(QString value);

    /**
     * @brief channels creates a list of Channel objects that can be used individually to
     * show or hide certain channels, and to retrieve the contents of each channel in a
     * node separately.
     *
     * Only layers have channels, masks do not, and calling channels on a Node that is a mask
     * will return an empty list.
     *
     * @return the list of channels ordered in by position of the channels in pixel position
     */
    QList<Channel*> channels() const;

    /**
     * Return a list of child nodes of the current node. The nodes are ordered from the bottommost up.
     * The function is not recursive.
     */
    QList<Node*> childNodes() const;
    void setChildNodes(QList<Node*> value);

    /**
     * colorDepth A string describing the color depth of the image:
     * <ul>
     * <li>U8: unsigned 8 bits integer, the most common type</li>
     * <li>U16: unsigned 16 bits integer</li>
     * <li>F16: half, 16 bits floating point. Only available if Krita was built with OpenEXR</li>
     * <li>F32: 32 bits floating point</li>
     * </ul>
     * @return the color depth.
     */
    QString colorDepth() const;

    /**
     * @brief colorModel retrieve the current color model of this document:
     * <ul>
     * <li>A: Alpha mask</li>
     * <li>RGBA: RGB with alpha channel (The actual order of channels is most often BGR!)</li>
     * <li>XYZA: XYZ with alpha channel</li>
     * <li>LABA: LAB with alpha channel</li>
     * <li>CMYKA: CMYK with alpha channel</li>
     * <li>GRAYA: Gray with alpha channel</li>
     * <li>YCbCrA: YCbCr with alpha channel</li>
     * </ul>
     * @return the internal color model string.
     */
    QString colorModel() const;

    /**
     * @return the name of the current color profile
     */
    QString colorProfile() const;

    /**
     * @brief setColorProfile set the color profile of the image to the given profile. The profile has to
     * be registered with krita and be compatible with the current color model and depth; the image data
     * is <i>not</i> converted.
     * @param colorProfile
     */
    void setColorProfile(const QString &colorProfile);

    /**
     * @brief setColorSpace convert the node to the given colorspace
     * @param colorModel A string describing the color model of the node:
     * <ul>
     * <li>A: Alpha mask</li>
     * <li>RGBA: RGB with alpha channel (The actual order of channels is most often BGR!)</li>
     * <li>XYZA: XYZ with alpha channel</li>
     * <li>LABA: LAB with alpha channel</li>
     * <li>CMYKA: CMYK with alpha channel</li>
     * <li>GRAYA: Gray with alpha channel</li>
     * <li>YCbCrA: YCbCr with alpha channel</li>
     * </ul>
     * @param colorDepth A string describing the color depth of the image:
     * <ul>
     * <li>U8: unsigned 8 bits integer, the most common type</li>
     * <li>U16: unsigned 16 bits integer</li>
     * <li>F16: half, 16 bits floating point. Only available if Krita was built with OpenEXR</li>
     * <li>F32: 32 bits floating point</li>
     * </ul>
     * @param colorProfile a valid color profile for this color model and color depth combinatiojn.
     */
    void setColorSpace(const QString &colorModel, const QString &colorDepth, const QString &colorProfile);

    QString colorLabel() const;
    void setColorLabel(QString value);


    bool inheritAlpha() const;
    void setInheritAlpha(bool value);

    bool locked() const;
    void setLocked(bool value);

    QString name() const;
    void setName(QString value);

    int opacity() const;
    void setOpacity(int value);

    Node* parentNode() const;
    void setParentNode(Node* value);

    QString type() const;
    void setType(QString value);

    bool visible() const;
    void setVisible(bool value);

    InfoObject* metaDataInfo() const;
    void setMetaDataInfo(InfoObject* value);

    Generator* generator() const;
    void setGenerator(Generator* value);

    Filter* filter() const;
    void setFilter(Filter* value);

    Transformation* transformation() const;
    void setTransformation(Transformation* value);

    Selection* selection() const;
    void setSelection(Selection* value);

    QString fileName() const;
    void setFileName(QString value);

    QByteArray pixelData() const;
    void setPixelData(QByteArray value);

    QRect bounds() const;


public Q_SLOTS:

    void move(int x, int y);

    void moveToParent(Node *parent);

    void remove();

    Node* duplicate();

    /**
     * @brief save exports the given node with this filename. The extension of the filename determins the filetype.
     * @param filename the filename including extension
     * @param xRes the horizontal resolution in pixels per pt (there are 72 pts in an inch)
     * @param yRes the horizontal resolution in pixels per pt (there are 72 pts in an inch)
     * @return true if saving succeeded, false if it failed.
     */
    bool save(const QString &filename, double xRes, double yRes);

private:

    friend class Filter;
    friend class Document;
    /**
     * @brief paintDevice gives access to the internal paint device of this Node
     * @return the paintdevice or 0 if the node does not have an editable paint device.
     */
    KisPaintDeviceSP paintDevice() const;
    KisImageSP image() const;
    KisNodeSP node() const;

    struct Private;
    Private *d;

};

#endif // LIBKIS_NODE_H
