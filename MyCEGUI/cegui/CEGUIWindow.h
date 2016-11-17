#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIVector.h"
#include "CEGUIRect.h"
#include "CEGUISize.h"
#include "CEGUIEventSet.h"
#include "CEGUIPropertySet.h"
#include "CEGUISystem.h"
#include "CEGUIInputEvent.h"
#include "CEGUIWindowProperties.h"
#include "CEGUIUDim.h"
#include "CEGUIWindowRenderer.h"
#include "CEGUITextUtils.h"
#include "CEGUIBasicRenderedStringParser.h"
#include "CEGUIDefaultRenderedStringParser.h"
#include <vector>
#include <set>



namespace CEGUI
{

//垂直对齐方式
enum VerticalAlignment
{
    VA_TOP,
    VA_CENTRE,
    VA_BOTTOM
};

//水平对齐方式
enum HorizontalAlignment
{
    HA_LEFT,
    HA_CENTRE,
    HA_RIGHT
};

/*!
\brief
    Enumerated type used for specifying Window::update mode to be used.  Note
    that the setting specified will also have an effect on child window
    content; for WUM_NEVER and WUM_VISIBLE, if the parent's update function is
    not called, then no child window will have it's update function called
    either - even if it specifies WUM_ALWAYS as it's WindowUpdateMode.
*/
//Window::update的调用方式，会影响子窗口
enum WindowUpdateMode
{
    //! Always call the Window::update function for this window.
    WUM_ALWAYS,
    //! Never call the Window::update function for this window.
    WUM_NEVER,
    //! Only call the Window::update function for this window if it is visible.
    WUM_VISIBLE
};


class Window : public PropertySet, public EventSet
{
public:
    /*************************************************************************
        Event name constants
    *************************************************************************/
    //! Namespace for global events
    static const String EventNamespace;
    static const String EventWindowUpdated;
    //window指针是父窗口的指针
    static const String EventParentSized;
    //窗口大小改变
    static const String EventSized;
    //窗口位置发生改变
    static const String EventMoved;
    static const String EventTextChanged;
    static const String EventFontChanged;
    static const String EventAlphaChanged;
    static const String EventIDChanged;
	//window获得焦点的窗口，otherwindow失去焦点的窗口
    static const String EventActivated;
    //window失去焦点的窗口，window获得焦点的窗口
    static const String EventDeactivated;
    static const String EventShown;
    static const String EventHidden;
    static const String EventEnabled;
    static const String EventDisabled;
    static const String EventClippedByParentChanged;
    static const String EventDestroyedByParentChanged;
    static const String EventInheritsAlphaChanged;
    static const String EventAlwaysOnTopChanged;
    static const String EventInputCaptureGained;
	//如果是自身释放了输入，window指向的是自身，如果不是，window指向的是获得输入的窗口
    static const String EventInputCaptureLost;
    /** Event fired when rendering of the Window has started.  In this context
     * 'rendering' is the population of the GeometryBuffer with geometry for the
     * window, not the actual rendering of that GeometryBuffer content to the 
     * display.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the Window whose rendering has started.
     */
    static const String EventRenderingStarted;
    static const String EventRenderingEnded;
    //window指向子窗口
    static const String EventChildAdded;
    static const String EventChildRemoved;
    static const String EventDestructionStarted;
    static const String EventZOrderChanged;
    /** Event fired when a DragContainer is dragged in to the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer has
     * been dragged (the receiving window) and DragDropEventArgs::dragDropItem
     * set to the DragContainer that was dragged in to the receiving window's
     * area.
     */
    static const String EventDragDropItemEnters;
    /** Event fired when a DragContainer is dragged out of the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer has
     * been dragged out of (the receiving window) and
     * DragDropEventArgs::dragDropItem set to the DragContainer that was dragged
     * out of the receiving window's area.
     */
    static const String EventDragDropItemLeaves;
    /** Event fired when a DragContainer is dropped within the window's area.
     * Handlers are passed a const DragDropEventArgs reference with
     * WindowEventArgs::window set to the window over which a DragContainer was
     * dropped (the receiving window) and DragDropEventArgs::dragDropItem set to
     * the DragContainer that was dropped within the receiving window's area.
     */
    static const String EventDragDropItemDropped;
    static const String EventVerticalAlignmentChanged;
    static const String EventHorizontalAlignmentChanged;
    static const String EventWindowRendererAttached;
    static const String EventWindowRendererDetached;
    static const String EventRotated;
    static const String EventNonClientChanged;
    static const String EventTextParsingChanged;
    static const String EventMarginChanged;
    static const String EventMouseEntersArea;
    static const String EventMouseLeavesArea;
    //在子窗口区域上时，父窗口不会触发此事件
    static const String EventMouseEnters;
    static const String EventMouseLeaves;
    static const String EventMouseMove;
    static const String EventMouseWheel;
    static const String EventMouseButtonDown;
    static const String EventMouseButtonUp;
    //在一定的时间间隔down and up
    static const String EventMouseClick;
    static const String EventMouseDoubleClick;
    static const String EventMouseTripleClick;
    //获得输入的窗口触发事件
    static const String EventKeyDown;
    static const String EventKeyUp;
	//UTF32格式的字符
    static const String EventCharacterKey;

    /*************************************************************************
        Child Widget name suffix constants
    *************************************************************************/
    //! Widget name suffix for automatically created tooltip widgets.
    static const String TooltipNameSuffix;
    //! Something that all generated widgets will have in their names.
    static const String AutoWidgetNameSuffix;

    Window(const String& type, const String& name);
	//virtual
    virtual ~Window(void);
    const String& getType(void) const;
    const String& getName(void) const  {return d_name;}
    bool    isDestroyedByParent(void) const    {return d_destroyedByParent;}
    bool isAlwaysOnTop(void) const  {return d_alwaysOnTop;}
	//localOnly是否综合考虑父窗口的情况
    bool isDisabled(bool localOnly = false) const;
    bool isVisible(bool localOnly = false) const;
	//父窗口不激活，子窗口也不激活
    bool isActive(void) const;
    bool isClippedByParent(void) const {return d_clippedByParent;}
    uint getID(void) const {return d_ID;}
    size_t getChildCount(void) const  {return d_children.size();}
	//没有查重操作，第一个找到的
    bool isChild(const String& name) const;
	//没有查重操作，第一个找到的
    bool isChild(uint ID) const;
	//递归在子窗口中查找
    bool isChildRecursive(uint ID) const;
    bool isChild(const Window* window) const;
	//不判重，第一个找到的
    //exception UnknownObjectException thrown if no window named \a name is attached to this Window.
    Window* getChild(const String& name) const;
	//不判重，第一个找到的
    //exception UnknownObjectException thrown if no window with the ID code \a ID is attached to this Window.
    Window* getChild(uint ID) const;
	//调用代价高，不抛出异常，返回0
    Window* getChildRecursive(const String& name) const;
    Window* getChildRecursive(uint ID) const;
    Window* getChildAtIdx(size_t idx) const {return d_children[idx];}
	//找到最上层的激活子窗口
    Window* getActiveChild(void);
    const Window* getActiveChild(void) const;
	//递归查找是否是祖先
    bool isAncestor(const String& name) const;
    bool isAncestor(uint ID) const;
    bool isAncestor(const Window* window) const;
    Font* getFont(bool useDefault = true) const;
    const String& getText(void) const {return d_textLogical;}
    //! return text string with \e visual ordering of glyphs.
    const String& getTextVisual() const;
    bool inheritsAlpha(void) const  {return d_inheritsAlpha;}
    //不是渲染使用的alpha
    float getAlpha(void) const  {return d_alpha;}
	//渲染使用的alpha
    float getEffectiveAlpha(void) const;
    Rect getUnclippedOuterRect() const;
    Rect getUnclippedInnerRect() const;
	//综合了上面两个函数
    Rect getUnclippedRect(const bool inner) const;
	//剪切窗口
    Rect getOuterRectClipper() const;
	//剪切窗口
    Rect getInnerRectClipper() const;
	//综合上面两个函数
    Rect getClipRect(const bool non_client = false) const;
    Rect getHitTestRect() const;

    /*!
    \brief
        Return a Rect that describes the area that is used to position
        and - for scale values - size child content attached to this Window.

        By and large the area returned here will be the same as the unclipped
        inner rect (for client content) or the unclipped outer rect (for non
        client content), although certain advanced uses will require
        alternative Rects to be returned.

    \note
        The behaviour of this function is modified by overriding the
        protected Window::getClientChildWindowContentArea_impl and/or
        Window::getNonClientChildWindowContentArea_impl functions.

    \param non_client
        - true to return the non-client child content area.
        - false to return the client child content area (default).
    */
    Rect getChildWindowContentArea(const bool non_client = false) const;

    /*!
    \brief
        Return a Rect object that describes, unclipped, the inner rectangle for
        this window.  The inner rectangle is typically an area that excludes
        some frame or other rendering that should not be touched by subsequent
        rendering.

    \return
        Rect object that describes, in unclipped screen pixel co-ordinates, the
        window object's inner rect area.

    \note
        This function is going to change from public visibility to pretected.
        All code accessing the area rects via external code should be using the
        regular getUnclippedInnerRect function.
    */
    virtual Rect getUnclippedInnerRect_impl(void) const;
    static Window* getCaptureWindow(void)   {return d_captureWindow;}
    bool isCapturedByThis(void) const   {return getCaptureWindow() == this;}
    bool isCapturedByAncestor(void) const
    {return isAncestor(getCaptureWindow());}
    bool isCapturedByChild(void) const  {return isChild(getCaptureWindow());}
	//virtual
    virtual bool isHit(const Vector2& position,
                       const bool allow_disabled = false) const;
	//没有子窗口返回0
    Window* getChildAtPosition(const Vector2& position) const;
	//子窗口需要能接受点击事件，没有返回0
    Window* getTargetChildAtPosition(const Vector2& position, 
                                     const bool allow_disabled = false) const;
    Window* getParent(void) const   {return d_parent;}
	//进入窗口会被绘制的鼠标图像，返回NULL表示没有鼠标会被绘制
    const Image* getMouseCursor(bool useDefault = true) const;
    Size getPixelSize(void) const    { return d_pixelSize; }
    void* getUserData(void) const   {return d_userData;}

    /*!
    \brief
        Return whether this window is set to restore old input capture when it
        loses input capture.

        This is only really useful for certain sub-components for widget
        writers.

    \return
        - true if the window will restore the previous capture window when it
          loses input capture.
        - false if the window will set the capture window to NULL when it loses
          input capture (this is the default behaviour).
    */
    bool restoresOldCapture(void) const     {return d_restoreOldCapture;}

    /*!
    \brief
        Return whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \return
        - true if z-order changes are enabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind work normally.
        - false: z-order changes are disabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind are ignored.
    */
    bool isZOrderingEnabled(void) const;
    bool wantsMultiClickEvents(void) const;
    bool isMouseAutoRepeatEnabled(void) const;

    /*!
    \brief
        Return the current auto-repeat delay setting for this window.

    \return
        float value indicating the delay, in seconds, defore the first repeat
        mouse button down event will be triggered when autorepeat is enabled.
    */
    float getAutoRepeatDelay(void) const;

    /*!
    \brief
        Return the current auto-repeat rate setting for this window.

    \return
        float value indicating the rate, in seconds, at which repeat mouse
        button down events will be generated after the initial delay has
        expired.
    */
    float getAutoRepeatRate(void) const;
	//是否将输入分发到子窗口
    bool distributesCapturedInputs(void) const;
    bool isUsingDefaultTooltip(void) const;
    Tooltip* getTooltip(void) const;
    String getTooltipType(void) const;
    const String& getTooltipText(void) const;
	//TooltipText是否继承父窗口
    bool inheritsTooltipText(void) const;

    /*!
    \brief
        Return whether this window will rise to the top of the z-order when
        clicked with the left mouse button.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \return
        - true if the window will come to the top of other windows when the left
          mouse button is pushed within its area.
        - false if the window does not change z-order position when the left
          mouse button is pushed within its area.
     */
    bool isRiseOnClickEnabled(void) const   { return d_riseOnClick; }
	//窗口类名
    bool testClassName(const String& class_name) const
    {return testClassName_impl(class_name);}
    VerticalAlignment getVerticalAlignment() const  {return d_vertAlign;}
    HorizontalAlignment getHorizontalAlignment() const  {return d_horzAlign;}

    /*!
    \brief
        Return the GeometryBuffer object for this Window.

    \return
        Reference to the GeometryBuffer object for this Window.
    */
    GeometryBuffer& getGeometryBuffer();
    const String& getLookNFeel() const;
    bool getModalState(void) const
    {return(System::getSingleton().getModalTarget() == this);}
    //exception UnknownObjectException thrown if a user string named \a name does not exist.
    const String& getUserString(const String& name) const;
    bool isUserStringDefined(const String& name) const;

    /*!
    \brief
        Returns the active sibling window.

        This searches the immediate children of this window's parent, and
        returns a pointer to the active window.  The method will return this if
        we are the immediate child of our parent that is active.  If our parent
        is not active, or if no immediate child of our parent is active then 0
        is returned.  If this window has no parent, and this window is not
        active then 0 is returned, else this is returned.

    \return
        A pointer to the immediate child window attached to our parent that is
        currently active, or 0 if no immediate child of our parent is active.
    */
    Window* getActiveSibling();
    Size getParentPixelSize(void) const;
    float getParentPixelWidth(void) const;
    float getParentPixelHeight(void) const;
	//是否对鼠标事件透明
    bool isMousePassThroughEnabled(void) const  {return d_mousePassThroughEnabled;}
	//是否是自动子窗口
    bool isAutoWindow(void) const   {return d_autoWindow;}
    bool isWritingXMLAllowed(void) const    {return d_allowWriteXML;}
    EventSet::Iterator getEventIterator() const;
    PropertySet::Iterator getPropertyIterator() const;

    /*!
    \brief
        Returns whether this Window object will receive events generated by
        the drag and drop support in the system.

    \return
        - true if the Window is enabled as a drag and drop target.
        - false if the window is not enabled as a drag and drop target.
    */
    bool isDragDropTarget() const;

    /*!
    \brief
        Fill in the RenderingContext \a ctx with details of the RenderingSurface
        where this Window object should normally do it's rendering.
    */
    void getRenderingContext(RenderingContext& ctx) const;

    //! implementation of the default getRenderingContext logic.
    virtual void getRenderingContext_impl(RenderingContext& ctx) const;

    /*!
    \brief
        return the RenderingSurface currently set for this window.  May return
        0.
    */
    RenderingSurface* getRenderingSurface() const;

    /*!
    \brief
        return the RenderingSurface that will be used by this window as the
        target for rendering.
    */
    RenderingSurface& getTargetRenderingSurface() const;

    /*!
    \brief
        Returns whether \e automatic use of an imagery caching RenderingSurface
        (i.e. a RenderingWindow) is enabled for this window.  The reason we
        emphasise 'automatic' is because the client may manually set a
        RenderingSurface that does exactly the same job.

    \return
        - true if automatic use of a caching RenderingSurface is enabled.
        - false if automatic use of a caching RenderTarget is not enabled.
    */
    bool isUsingAutoRenderingSurface() const;

    /*!
    \brief
        Returns the window at the root of the hierarchy starting at this
        Window.  The root window is defined as the first window back up the
        hierarchy that has no parent window.

    \return
        A pointer to the root window of the hierarchy that this window is
        attched to.
    */
    const Window* getRootWindow() const;
    Window* getRootWindow();

    //! return the rotations set for this window.
    const Vector3& getRotation() const;

    /*!
    \brief
        Return whether the Window is a non-client window.

        A non-client window is clipped, positioned and sized according to the
        parent window's full area as opposed to just the inner rect area used
        for normal client windows.

    \return
        - true if the window should is clipped, positioned and sized according
        to the full area rectangle of it's parent.
        - false if the window is be clipped, positioned and sized according
        to the inner rect area of it's parent.
    */
    bool isNonClientWindow() const;
    //exception AlreadyExistsException thrown if a Window named \a new_name already exists in the system.
    void rename(const String& new_name);

    /*!
    \brief
        Initialises the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled
        automatically by the WindowManager.

    \return
        Nothing
    */
    virtual void initialiseComponents(void) {}
    void setDestroyedByParent(bool setting);
    void setAlwaysOnTop(bool setting);
    void setEnabled(bool setting);
    void enable(void)   {setEnabled(true);}
    void disable(void)  {setEnabled(false);}
	//会自动使激活窗口变为非激活窗口，激活一个窗口会使窗口visible
    void setVisible(bool setting);
	//不会让窗口变为激活状态
    void show(void)     {setVisible(true);}
	//会取消激活状态
    void hide(void)     {setVisible(false);}
    void activate(void);
    void deactivate(void);
    void setClippedByParent(bool setting);
    void setID(uint ID);
    void setText(const String& text);

    /*!
    \brief
        Insert the text string \a text into the current text string for the
        Window object at the position specified by \a position.
     
    \param text
        String object holding the text that is to be inserted into the Window
        object's current text string.
     
    \param position
        The characted index position where the string \a text should be
        inserted.
    */
    void insertText(const String& text, const String::size_type position);

    /*!
    \brief
        Append the string \a text to the currect text string for the Window
        object.
     
    \param text
        String object holding the text that is to be appended to the Window
        object's current text string.
    */
    void appendText(const String& text);
	//NULL表示使用默认字体
    void setFont(Font* font);
	//""表示使用默认字体
    //exception UnknownObjectException thrown if the specified Font is unknown within the system.
    void setFont(const String& name);
	//取消与原父窗口的关联
    //exception UnknownObjectException thrown if no Window named \a name exists.
    //exception InvalidRequestException thrown if Window \a name is an ancestor of this Window, to prevent cyclic Window structures.
    void addChildWindow(const String& name);
    //exception InvalidRequestException thrown if Window \a window is an ancestor of this Window, to prevent cyclic Window structures.
    void addChildWindow(Window* window);
    void removeChildWindow(const String& name);
    void removeChildWindow(Window* window);
    void removeChildWindow(uint ID);
	//所有的窗口非为两类，第一类non always-on-top，第二类always-on-top
	//第一类，移到所有non always-on-top兄弟窗口的顶端
	//第二类，移到所有兄弟窗口的顶端
    void moveToFront();
	//所有的窗口非为两类，第一类non always-on-top，第二类always-on-top
	//第一类，移到所有兄弟窗口的底端
	//第二类，移到所有always-on-top兄弟窗口的底端
    void moveToBack();

    /*!
    \brief
        Move this window immediately above it's sibling \a window in the z order.

        No action will be taken under the following conditions:
        - \a window is 0.
        - \a window is not a sibling of this window.
        - \a window and this window have different AlwaysOnTop settings.
        - z ordering is disabled for this window.

    \param window
        The sibling window that this window will be moved in front of.
    */
    void moveInFront(const Window* const window);

    /*!
    \brief
        Move this window immediately behind it's sibling \a window in the z
        order.

        No action will be taken under the following conditions:
        - \a window is 0.
        - \a window is not a sibling of this window.
        - \a window and this window have different AlwaysOnTop settings.
        - z ordering is disabled for this window.

    \param window
        The sibling window that this window will be moved behind.
    */
    void moveBehind(const Window* const window);
	//返回是否成功，非active窗口无法捕获输出
    bool captureInput(void);
    void releaseInput(void);

    /*!
    \brief
        Set whether this window will remember and restore the previous window
        that had inputs captured.

    \param setting
        - true: The window will remember and restore the previous capture
          window.  The CaptureLost event is not fired on the previous window
          when this window steals input capture.  When this window releases
          capture, the old capture window is silently restored.

        - false: Input capture works as normal, each window losing capture is
          signalled via CaptureLost, and upon the final release of capture, no
          previous setting is restored (this is the default behaviour).

    \return
        Nothing
    */
    void setRestoreCapture(bool setting);

    /*!
    \brief
        Set the current alpha value for this window.

    \note
        The alpha value set for any given window may or may not be the final
        alpha value that is used when rendering.  All window objects, by
        default, inherit alpha from thier parent window(s) - this will blend
        child windows, relatively, down the line of inheritance.  This behaviour
        can be overridden via the setInheritsAlpha() method.  To return the true
        alpha value that will be applied when rendering, use the
        getEffectiveAlpha() method.

    \param alpha
        The new alpha value for the window.
        Value should be between 0.0f and 1.0f.

    \return
        Nothing
    */
    void setAlpha(float alpha);

    /*!
    \brief
        Sets whether this Window will inherit alpha from its parent windows.

    \param setting
        - true if the Window should use inherited alpha.
        - false if the Window should have an independant alpha value.

    \return
        Nothing
    */
    void setInheritsAlpha(bool setting);
	//使窗口重绘
    void invalidate(void);

    /*!
    \brief
        Invalidate this window and - dependant upon \a recursive - all child
        content, causing affected windows to be redrawn during the next
        rendering pass.

    \param recursive
        Boolean value indicating whether attached child content should also be
        invalidated.
        - true will cause all child content to be invalidated also.
        - false will just invalidate this single window.

    \return
        Nothing
    */
    void invalidate(const bool recursive);
    void setMouseCursor(const Image* image);
    void setMouseCursor(MouseCursorImage image);
    //exception UnknownObjectException
    //    thrown if \a imageset is not known, or if \a imageset contains no Image
    //    named \a image_name.
    void setMouseCursor(const String& imageset, const String& image_name);
    void setUserData(void* user_data)   {d_userData = user_data;}

    /*!
    \brief
        Set whether z-order changes are enabled or disabled for this Window.

    \note
        This is distinguished from the is/setRiseOnClickEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \param setting
        - true if z-order changes are enabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind work normally.
        - false: z-order changes are disabled for this window.
          moveToFront, moveToBack, moveInFront and moveBehind are ignored.

    \return
        Nothing.
    */
    void    setZOrderingEnabled(bool setting);
    void setWantsMultiClickEvents(bool setting);
    void setMouseAutoRepeatEnabled(bool setting);
    void setAutoRepeatDelay(float delay);
    void setAutoRepeatRate(float rate);
    void setDistributesCapturedInputs(bool setting);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemEnters(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemLeaves(DragContainer* item);

    /*!
    \brief
        Internal support method for drag & drop.  You do not normally call
        this directly from client code.  See the DragContainer class.
    */
    void notifyDragDropItemDropped(DragContainer* item);

    /*!
    \brief
        Internal destroy method which actually just adds the window and any
        parent destructed child windows to the dead pool.

        This is virtual to allow for specialised cleanup which may be required
        in some advanced cases.  If you override this for the above reason, you
        MUST call this base class version.

    \note
        You never have to call this method yourself, use WindowManager to
        destroy your Window objects (which will call this for you).
    */
    virtual void destroy(void);

    /*!
    \brief
        Set the custom Tooltip object for this Window.  This value may be 0 to
        indicate that the Window should use the system default Tooltip object.

    \param tooltip
        Pointer to a valid Tooltip based object which should be used as the
        tooltip for this Window, or 0 to indicate that the Window should use the
        system default Tooltip object.  Note that when passing a pointer to a
        Tooltip object, ownership of the Tooltip does not pass to this Window
        object.

    \return
        Nothing.
    */
    void setTooltip(Tooltip* tooltip);

    /*!
    \brief
        Set the custom Tooltip to be used by this Window by specifying a Window
        type.

        The Window will internally attempt to create an instance of the
        specified window type (which must be derived from the base Tooltip
        class).  If the Tooltip creation fails, the error is logged and the
        Window will revert to using either the existing custom Tooltip or the
        system default Tooltip.

    \param tooltipType
        String object holding the name of the Tooltip based Window type which
        should be used as the Tooltip for this Window.

    \return
        Nothing.
    */
    void setTooltipType(const String& tooltipType);

    /*!
    \brief
        Set the tooltip text for this window.

    \param tip
        String object holding the text to be displayed in the tooltip for this
        Window.

    \return
        Nothing.
    */
    void setTooltipText(const String& tip);

    /*!
    \brief
        Set whether this window inherits Tooltip text from its parent when its
        own tooltip text is not set.

    \param setting
        - true if the window should inherit tooltip text from its parent when
          its own text is not set.
        - false if the window should not inherit tooltip text from its parent
          (and so show no tooltip when no text is set).

    \return
        Nothing.
     */
    void setInheritsTooltipText(bool setting);

    /*!
    \brief
        Set whether this window will rise to the top of the z-order when clicked
        with the left mouse button.

    \note
        This is distinguished from the is/setZOrderingEnabled setting in that
        if rise on click is disabled it only affects the users ability to affect
        the z order of the Window by clicking the mouse; is still possible to
        programatically alter the Window z-order by calling the moveToFront,
        moveToBack, moveInFront and moveBehind member functions.  Whereas if z
        ordering is disabled those functions are also precluded from affecting
        the Window z position.

    \param setting
        - true if the window should come to the top of other windows when the
          left mouse button is pushed within its area.
        - false if the window should not change z-order position when the left
          mouse button is pushed within its area.

    \return
        Nothing.
     */
    void setRiseOnClickEnabled(bool setting)    { d_riseOnClick = setting; }
    void setVerticalAlignment(const VerticalAlignment alignment);
    void setHorizontalAlignment(const HorizontalAlignment alignment);
    //exception UnknownObjectException thrown if the look'n'feel specified by \a look does not exist.
    virtual void setLookNFeel(const String& look);
    void setModalState(bool state);

    /*!
    \brief
        method called to perform extended laying out of attached child windows.

        The system may call this at various times (like when it is resized for
        example), and it may be invoked directly where required.

    \return
        Nothing.
    */
    virtual void performChildWindowLayout();
    void setUserString(const String& name, const String& value);
    void setArea(const UDim& xpos, const UDim& ypos, const UDim& width, const UDim& height);
    void setArea(const UVector2& pos, const UVector2& size);
    void setArea(const URect& area);
    void setPosition(const UVector2& pos);
    void setXPosition(const UDim& x);
    void setYPosition(const UDim& y);
    void setSize(const UVector2& size);
    void setWidth(const UDim& width);
    void setHeight(const UDim& height);
    void setMaxSize(const UVector2& size);
    void setMinSize(const UVector2& size);
    const URect& getArea() const;
    const UVector2& getPosition() const;
    const UDim& getXPosition() const;
    const UDim& getYPosition() const;
    UVector2 getSize() const;
    UDim getWidth() const;
    UDim getHeight() const;
    const UVector2& getMaxSize() const;
    const UVector2& getMinSize() const;
	//渲染自己以及所有的子窗口
    void render();

    /*!
    \brief
        Cause window to update itself and any attached children.  Client code
        does not need to call this method; to ensure full, and proper updates,
        call the injectTimePulse methodname method provided by the System class.

    \note
        The update order is such that 'this' window is updated prior to any
        child windows, this is so that child windows that access the parent in
        their update code get the correct updated state.

    \param elapsed
        float value indicating the number of seconds passed since the last
        update.

    \return
        Nothing.
    */
	//virtual
	//窗口更新自己，例如定时器
    virtual void update(float elapsed);

    /*!
    \brief
        Writes an xml representation of this window object to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.

    \return
        Nothing.
    */
    virtual void writeXMLToStream(XMLSerializer& xml_stream) const;

    /*!
    \brief
        Sets the internal 'initialising' flag to true.
        This can be use to optimize initialisation of some widgets, and is called
        automatically by the layout XML handler when it has created a window.
        That is just after the window has been created, but before any children or
        properties are read.
    */
    virtual void beginInitialisation(void)     {d_initialising = true;}

    /*!
    \brief
        Sets the internal 'initialising' flag to false.
        This is called automatically by the layout XML handler when it is done
        creating a window. That is after all properties and children have been
        loaded and just before the next sibling gets created.
    */
    virtual void endInitialisation(void)       {d_initialising = false;}
    void setMousePassThroughEnabled(bool setting)   {d_mousePassThroughEnabled = setting;}
    void setWindowRenderer(const String& name);
	//没有返回0
    WindowRenderer* getWindowRenderer(void) const;
	//没有返回空字符串
    String getWindowRendererName(void) const;
    void setWritingXMLAllowed(bool allow)   {d_allowWriteXML = allow;}

    /*!
    \brief
        Inform the window, and optionally all children, that screen area
        rectangles have changed.

    \param recursive
        - true to recursively call notifyScreenAreaChanged on attached child
          Window objects.
        - false to just process \e this Window.
    */
    void notifyScreenAreaChanged(bool recursive = true);

    /*!
    \brief
        Changes the widget's falagard type, thus changing its look'n'feel and optionally its
        renderer in the process.

    \param type
        New look'n'feel of the widget

    \param type
        New renderer of the widget
    */
    void setFalagardType(const String& type, const String& rendererType = "");

    /*!
    \brief
        Specifies whether this Window object will receive events generated by
        the drag and drop support in the system.

    \param setting
        - true to enable the Window as a drag and drop target.
        - false to disable the Window as a drag and drop target.
    */
    void setDragDropTarget(bool setting);

    /*!
    \brief
        Set the RenderingSurface to be associated with this Window, or 0 if
        none is required.
    \par
        If this function is called, and the option for automatic use of an
        imagery caching RenderingSurface is enabled, any automatically created
        RenderingSurface will be released, and the affore mentioned option will
        be disabled.
    \par
        If after having set a custom RenderingSurface you then subsequently
        enable the automatic use of an imagery caching RenderingSurface by
        calling setUsingAutoRenderingSurface, the previously set
        RenderingSurface will be disassociated from the Window.  Note that the
        previous RenderingSurface is not destroyed or cleaned up at all - this
        is the job of whoever created that object initially.

    \param target
        Pointer to the RenderingSurface object to be associated with the window.
    */
    void setRenderingSurface(RenderingSurface* surface);

    /*!
    \brief
        Invalidate the chain of rendering surfaces from this window backwards to
        ensure they get properly redrawn - but doing the minimum amount of work
        possibe - next render.
    */
    void invalidateRenderingSurface();

    /*!
    \brief
        Sets whether \e automatic use of an imagery caching RenderingSurface
        (i.e. a RenderingWindow) is enabled for this window.  The reason we
        emphasise 'atutomatic' is because the client may manually set a
        RenderingSurface that does exactlythe same job.
    \par
        Note that this setting really only controls whether the Window
        automatically creates and manages the RenderingSurface, as opposed to
        the \e use of the RenderingSurface.  If a RenderingSurfaceis set for the
        Window it will be used regardless of this setting.
    \par
        Enabling this option will cause the Window to attempt to create a
        suitable RenderingSurface (which will actually be a RenderingWindow).
        If there is an existing RenderingSurface assocated with this Window, it
        will be removed as the Window's RenderingSurface
        <em>but not destroyed</em>; whoever created the RenderingSurface in the
        first place should take care of its destruction.
    \par
        Disabling this option will cause any automatically created
        RenderingSurface to be released.
    \par
        It is possible that the renderer in use may not support facilities for
        RenderingSurfaces that are suitable for full imagery caching.  If this
        is the case, then calling getRenderingSurface after enabling this option
        will return 0.  In these cases this option will still show as being
        'enabled', this is because Window \e settings should not be influenced
        by capabilities the renderer in use; for example, this enables correct
        XML layouts to be written from a Window on a system that does not
        support such RenderingSurfaces, so that the layout will function as
        preferred on systems that do.
    \par
        If this option is enabled, and the client subsequently assigns a
        different RenderingSurface to the Window, the existing automatically
        created RenderingSurface will be released and this setting will be
        disabled.

    \param setting
        - true to enable automatic use of an imagery caching RenderingSurface.
        - false to disable automatic use of an imagery caching RenderingSurface.
    */
    void setUsingAutoRenderingSurface(bool setting);

    //! set the rotations for this window.
    void setRotation(const Vector3& rotation);

    /*!
    \brief
        Set whether the Window is a non-client window.

        A non-client window is clipped, positioned and sized according to the
        parent window's full area as opposed to just the inner rect area used
        for normal client windows.

    \param setting
        - true if the window should be clipped, positioned and sized according
        to the full area rectangle of it's parent.
        - false if the window should be clipped, positioned and sized according
        to the inner rect area of it's parent.
    */
    void setNonClientWindow(const bool setting);

    //! Return the parsed RenderedString object for this window.
    const RenderedString& getRenderedString() const;
    //! Return a pointer to any custom RenderedStringParser set, or 0 if none.
    RenderedStringParser* getCustomRenderedStringParser() const;
    //! Set a custom RenderedStringParser, or 0 to remove an existing one.
    void setCustomRenderedStringParser(RenderedStringParser* parser);
    //! return the active RenderedStringParser to be used
    virtual RenderedStringParser& getRenderedStringParser() const;
    //! return whether text parsing is enabled for this window.
    bool isTextParsingEnabled() const;
    //! set whether text parsing is enabled for this window.
    void setTextParsingEnabled(const bool setting);

    //! set margin
    virtual void setMargin(const UBox& margin);
    //! retrieves currently set margin
    const UBox& getMargin() const;

    //! return Vector2 \a pos after being fully unprojected for this Window.
    Vector2 getUnprojectedPosition(const Vector2& pos) const;

    //! return the pointer to the BiDiVisualMapping for this window, if any.
    const BiDiVisualMapping* getBiDiVisualMapping() const
        {return d_bidiVisualMapping;}

    //! Add the named property to the XML ban list for this window.
    void banPropertyFromXML(const String& property_name);

    //! Remove the named property from the XML ban list for this window.
    void unbanPropertyFromXML(const String& property_name);

    //! Return whether the named property is banned from XML
    bool isPropertyBannedFromXML(const String& property_name) const;

    //! Add the given property to the XML ban list for this window.
    void banPropertyFromXML(const Property* property);

    //! Remove the given property from the XML ban list for this window.
    void unbanPropertyFromXML(const Property* property);

    //! Return whether the given property is banned from XML
    bool isPropertyBannedFromXML(const Property* property) const;

    /*!
    \brief
        Set the window update mode.  This mode controls the behaviour of the
        Window::update member function such that updates are processed for
        this window (and therefore it's child content) according to the set
        mode.

    \note
        Disabling updates can have negative effects on the behaviour of CEGUI
        windows and widgets; updates should be disabled selectively and
        cautiously - if you are unsure of what you are doing, leave the mode
        set to WUM_ALWAYS.
    
    \param mode
        One of the WindowUpdateMode enumerated values indicating the mode to
        set for this Window.
    */
    void setUpdateMode(const WindowUpdateMode mode);

    /*!
    \brief
        Return the current window update mode that is set for this Window.
        This mode controls the behaviour of the Window::update member function
        such that updates are processed for this window (and therefore it's
        child content) according to the set mode.

    \note
        Disabling updates can have negative effects on the behaviour of CEGUI
        windows and widgets; updates should be disabled selectively and
        cautiously - if you are unsure of what you are doing, leave the mode
        set to WUM_ALWAYS.
    
    \return
        One of the WindowUpdateMode enumerated values indicating the current
        mode set for this Window.
    */
    WindowUpdateMode getUpdateMode() const;

    /*!
    \brief
        Set whether mouse input that is not directly handled by this Window
        (including it's event subscribers) should be propagated back to the
        Window's parent.

    \param enabled
        - true if unhandled mouse input should be propagated to the parent.
        - false if unhandled mouse input should not be propagated.
    */
    void setMouseInputPropagationEnabled(const bool enabled);
    bool isMouseInputPropagationEnabled() const;

    /*!
    \brief
        Clones this Window and returns the result

    \param 
        newName new name of the cloned window

    \param
        deepCopy if true, even children are copied (the old name prefix will
        be replaced with new name prefix)

    \return
        the cloned Window
    */
    Window* clone(const String& newName, const bool deepCopy = true) const;

    //! copies this widget's properties to given target widget
    virtual void clonePropertiesTo(Window& target) const;
    //! copies this widget's child widgets to given target widget
    virtual void cloneChildWidgetsTo(Window& target) const;

    /*!
    \brief
        Return the (visual) z index of the window on it's parent.
        
        The z index is a number that indicates the order that windows will be
        drawn (but is not a 'z co-ordinate', as such).  Higher numbers are in
        front of lower numbers.

        The number returned will not be stable, and generally should be used to
        compare with the z index of sibling windows (and only sibling windows)
        to discover the current z ordering of those windows.
    */
    size_t getZIndex() const;

    /*!
    \brief
        Return whether /a this Window is in front of the given window.

    \note
        Here 'in front' just means that one window is drawn after the other, it
        is not meant to imply that the windows are overlapping nor that one
        window is obscured by the other.
    */
    bool isInFront(const Window& wnd) const;

    /*!
    \brief
        Return whether /a this Window is behind the given window.

    \note
        Here 'behind' just means that one window is drawn before the other, it
        is not meant to imply that the windows are overlapping nor that one
        window is obscured by the other.
    */
    bool isBehind(const Window& wnd) const;

protected:
    // friend classes for construction / initialisation purposes (for now)
    friend class System;
    friend class WindowManager;

    /*************************************************************************
        Event trigger methods
    *************************************************************************/
    virtual void onSized(WindowEventArgs& e);
    virtual void onMoved(WindowEventArgs& e);
    virtual void onTextChanged(WindowEventArgs& e);
    virtual void onFontChanged(WindowEventArgs& e);
    virtual void onAlphaChanged(WindowEventArgs& e);
    virtual void onIDChanged(WindowEventArgs& e);
    virtual void onShown(WindowEventArgs& e);
    virtual void onHidden(WindowEventArgs& e);
    virtual void onEnabled(WindowEventArgs& e);
    virtual void onDisabled(WindowEventArgs& e);
    virtual void onClippingChanged(WindowEventArgs& e);
    virtual void onParentDestroyChanged(WindowEventArgs& e);
    virtual void onInheritsAlphaChanged(WindowEventArgs& e);
    virtual void onAlwaysOnTopChanged(WindowEventArgs& e);
    virtual void onCaptureGained(WindowEventArgs& e);
    virtual void onCaptureLost(WindowEventArgs& e);
    virtual void onRenderingStarted(WindowEventArgs& e);
    virtual void onRenderingEnded(WindowEventArgs& e);
    virtual void onZChanged(WindowEventArgs& e);
    virtual void onDestructionStarted(WindowEventArgs& e);
    virtual void onActivated(ActivationEventArgs& e);
    virtual void onDeactivated(ActivationEventArgs& e);
    virtual void onParentSized(WindowEventArgs& e);
    virtual void onChildAdded(WindowEventArgs& e);
    virtual void onChildRemoved(WindowEventArgs& e);
    virtual void onMouseEntersArea(MouseEventArgs& e);
    virtual void onMouseLeavesArea(MouseEventArgs& e);
    virtual void onMouseEnters(MouseEventArgs& e);
    virtual void onMouseLeaves(MouseEventArgs& e);
    virtual void onMouseMove(MouseEventArgs& e);
    virtual void onMouseWheel(MouseEventArgs& e);
    virtual void onMouseButtonDown(MouseEventArgs& e);
    virtual void onMouseButtonUp(MouseEventArgs& e);
    virtual void onMouseClicked(MouseEventArgs& e);
    virtual void onMouseDoubleClicked(MouseEventArgs& e);
    virtual void onMouseTripleClicked(MouseEventArgs& e);
    virtual void onKeyDown(KeyEventArgs& e);
    virtual void onKeyUp(KeyEventArgs& e);
    virtual void onCharacter(KeyEventArgs& e);
    virtual void onDragDropItemEnters(DragDropEventArgs& e);
    virtual void onDragDropItemLeaves(DragDropEventArgs& e);
    virtual void onDragDropItemDropped(DragDropEventArgs& e);
    virtual void onVerticalAlignmentChanged(WindowEventArgs& e);
    virtual void onHorizontalAlignmentChanged(WindowEventArgs& e);
    virtual void onWindowRendererAttached(WindowEventArgs& e);
    virtual void onWindowRendererDetached(WindowEventArgs& e);
    virtual void onRotated(WindowEventArgs& e);
    virtual void onNonClientChanged(WindowEventArgs& e);
    virtual void onTextParsingChanged(WindowEventArgs& e);

    virtual void onMarginChanged(WindowEventArgs& e);

    /*************************************************************************
        Implementation Functions
    *************************************************************************/
    /*!
    \brief
        Perform actual update processing for this Window.

    \param elapsed
        float value indicating the number of seconds elapsed since the last
        update call.

    \return
        Nothing.
    */
    virtual void updateSelf(float elapsed);

    /*!
    \brief
        Perform the actual rendering for this Window.

    \param ctx
        RenderingContext holding the details of the RenderingSurface to be
        used for the Window rendering operations.

    \return
        Nothing
    */
    virtual void drawSelf(const RenderingContext& ctx);

    /*!
    \brief
        Perform drawing operations concerned with generating and buffering
        window geometry.

    \note
        This function is a sub-function of drawSelf; it is provided to make it
        easier to override drawSelf without needing to duplicate large sections
        of the code from the default implementation.
    */
    void bufferGeometry(const RenderingContext& ctx);

    /*!
    \brief
        Perform drawing operations concerned with positioning, clipping and
        queueing of window geometry to RenderingSurfaces.

    \note
        This function is a sub-function of drawSelf and is provided to make it
        easier to override drawSelf without needing to duplicate large sections
        of the code from the default implementation.
    */
    void queueGeometry(const RenderingContext& ctx);

    /*!
    \brief
        Update the rendering cache.

        Populates the Window's GeometryBuffer ready for rendering.
    */
    virtual void populateGeometryBuffer()  {}
    virtual bool testClassName_impl(const String& class_name) const
    {
        if (class_name == "Window")   return true;
        return false;
    }
    void setParent(Window* parent);
    Size getSize_impl(const Window* window) const;

    /*!
    \brief
        Fires off a repeated mouse button down event for this window.
    */
    void generateAutoRepeatEvent(MouseButton button);

    /*!
    \brief
        Function used in checking if a WindowRenderer is valid for this window.

    \return
        Returns true if the given WindowRenderer class name is valid for this window.
        False if not.
    */
    virtual bool validateWindowRenderer(const String& name) const;

    /*!
    \brief
        Returns whether a property is at it's default value.
        This function is different from Property::isDefatult as it takes the assigned look'n'feel
        (if the is one) into account.
    */
    bool isPropertyAtDefault(const Property* property) const;

    /*!
    \brief
        Recursively inform all children that the clipping has changed and screen rects
        needs to be recached.
    */
    void notifyClippingChanged(void);

    //! helper to create and setup the auto RenderingWindow surface
    void allocateRenderingWindow();

    //! helper to clean up the auto RenderingWindow surface
    void releaseRenderingWindow();

    //! Helper to intialise the needed clipping for geometry and render surface.
    void initialiseClippers(const RenderingContext& ctx);

	//virtual
    virtual void cleanupChildren(void);
    virtual void addChild_impl(Window* wnd);
    virtual void removeChild_impl(Window* wnd);

    /*!
    \brief
        Notify 'this' and all siblings of a ZOrder change event
    */
    virtual void onZChange_impl(void);

    /*!
    \brief
        Add standard CEGUI::Window properties.
    */
    void    addStandardProperties(void);

    //virtual
    virtual bool moveToFront_impl(bool wasClicked);
    void setArea_impl(const UVector2& pos, const UVector2& size,
                      bool topLeftSizing = false, bool fireEvents = true);

    /*!
    \brief
        Add the given window to the drawing list at an appropriate position for
        it's settings and the required direction.  Basically, when \a at_back
        is false, the window will appear in front of all other windows with the
        same 'always on top' setting.  When \a at_back is true, the window will
        appear behind all other windows wih the same 'always on top' setting.

    \param wnd
        Window object to be added to the drawing list.

    \param at_back
        Indicates whether the window should be placed at the back of other
        windows in the same group. If this is false, the window is placed in
        front of other windows in the group.

    \return
        Nothing.
    */
    void addWindowToDrawList(Window& wnd, bool at_back = false);

    /*!
    \brief
        Removes the window from the drawing list.  If the window is not attached
        to the drawing list then nothing happens.

    \param wnd
        Window object to be removed from the drawing list.

    \return
        Nothing.
    */
    void removeWindowFromDrawList(const Window& wnd);

    /*!
    \brief
        Return whether the window is at the top of the Z-Order.  This will
        correctly take into account 'Always on top' windows as needed.

    \return
        - true if the Window is at the top of the z-order in relation to sibling
          windows with the same 'always on top' setting.
        - false if the Window is not at the top of the z-order in relation to
          sibling windows with the same 'always on top' setting.
    */
    bool isTopOfZOrder() const;

    /*!
    \brief
        Update position and clip region on this Windows geometry / rendering
        surface.
    */
    void updateGeometryRenderSettings();

    //! transfer RenderingSurfaces to be owned by our target RenderingSurface.
    void transferChildSurfaces();

    //! helper function for calculating clipping rectangles.
    Rect getParentElementClipIntersection(const Rect& unclipped_area) const;

    //! helper function to invalidate window and optionally child windows.
    void invalidate_impl(const bool recursive);

    //! helper to return whether the inner rect size has changed
    bool isInnerRectSizeChanged() const;

    /*!
    \brief
        Helper function to return the ancestor Window of /a wnd that is attached
        as a child to a window that is also an ancestor of /a this.  Returns 0
        if /a wnd and /a this are not part of the same hierachy.
     */
    const Window* getWindowAttachedToCommonAncestor(const Window& wnd) const;

    //! Default implementation of function to return Window outer rect area.
    virtual Rect getUnclippedOuterRect_impl() const;
    //! Default implementation of function to return Window outer clipper area.
    virtual Rect getOuterRectClipper_impl() const;
    //! Default implementation of function to return Window inner clipper area.
    virtual Rect getInnerRectClipper_impl() const;
    //! Default implementation of function to return Window hit-test area.
    virtual Rect getHitTestRect_impl() const;
    //! Default implementation of function to return non-client content area
    virtual Rect getNonClientChildWindowContentArea_impl() const;
    //! Default implementation of function to return client content area
    virtual Rect getClientChildWindowContentArea_impl() const;

    virtual int writePropertiesXML(XMLSerializer& xml_stream) const;
    virtual int writeChildWindowsXML(XMLSerializer& xml_stream) const;
    virtual bool writeAutoChildWindowXML(XMLSerializer& xml_stream) const;

    // constrain given UVector2 to window's min size, return if size changed.
    bool constrainUVector2ToMinSize(const Size& base_sz, UVector2& sz);
    // constrain given UVector2 to window's max size, return if size changed.
    bool constrainUVector2ToMaxSize(const Size& base_sz, UVector2& sz);

    /*************************************************************************
        Properties for Window base class
		都是静态变量，也就是所有的属性只有一份，只表示接口
    *************************************************************************/
    static  WindowProperties::Alpha             d_alphaProperty;
    static  WindowProperties::AlwaysOnTop       d_alwaysOnTopProperty;
    static  WindowProperties::ClippedByParent   d_clippedByParentProperty;
    static  WindowProperties::DestroyedByParent d_destroyedByParentProperty;
    static  WindowProperties::Disabled          d_disabledProperty;
    static  WindowProperties::Font              d_fontProperty;
    static  WindowProperties::ID                d_IDProperty;
    static  WindowProperties::InheritsAlpha     d_inheritsAlphaProperty;
    static  WindowProperties::MouseCursorImage  d_mouseCursorProperty;
    static  WindowProperties::RestoreOldCapture d_restoreOldCaptureProperty;
    static  WindowProperties::Text              d_textProperty;
    static  WindowProperties::Visible           d_visibleProperty;
    static  WindowProperties::ZOrderChangeEnabled   d_zOrderChangeProperty;
    static  WindowProperties::WantsMultiClickEvents d_wantsMultiClicksProperty;
    static  WindowProperties::MouseButtonDownAutoRepeat d_autoRepeatProperty;
    static  WindowProperties::AutoRepeatDelay   d_autoRepeatDelayProperty;
    static  WindowProperties::AutoRepeatRate    d_autoRepeatRateProperty;
    static  WindowProperties::DistributeCapturedInputs d_distInputsProperty;
    static  WindowProperties::CustomTooltipType d_tooltipTypeProperty;
    static  WindowProperties::Tooltip           d_tooltipProperty;
    static  WindowProperties::InheritsTooltipText d_inheritsTooltipProperty;
    static  WindowProperties::RiseOnClick       d_riseOnClickProperty;
    static  WindowProperties::VerticalAlignment   d_vertAlignProperty;
    static  WindowProperties::HorizontalAlignment d_horzAlignProperty;
    static  WindowProperties::UnifiedAreaRect   d_unifiedAreaRectProperty;
    static  WindowProperties::UnifiedPosition   d_unifiedPositionProperty;
    static  WindowProperties::UnifiedXPosition  d_unifiedXPositionProperty;
    static  WindowProperties::UnifiedYPosition  d_unifiedYPositionProperty;
    static  WindowProperties::UnifiedSize       d_unifiedSizeProperty;
    static  WindowProperties::UnifiedWidth      d_unifiedWidthProperty;
    static  WindowProperties::UnifiedHeight     d_unifiedHeightProperty;
    static  WindowProperties::UnifiedMinSize    d_unifiedMinSizeProperty;
    static  WindowProperties::UnifiedMaxSize    d_unifiedMaxSizeProperty;
    static  WindowProperties::MousePassThroughEnabled   d_mousePassThroughEnabledProperty;
    static  WindowProperties::WindowRenderer    d_windowRendererProperty;
    static  WindowProperties::LookNFeel         d_lookNFeelProperty;
    static  WindowProperties::DragDropTarget    d_dragDropTargetProperty;
    static  WindowProperties::AutoRenderingSurface d_autoRenderingSurfaceProperty;
    static  WindowProperties::Rotation d_rotationProperty;
    static  WindowProperties::XRotation d_xRotationProperty;
    static  WindowProperties::YRotation d_yRotationProperty;
    static  WindowProperties::ZRotation d_zRotationProperty;
    static  WindowProperties::NonClient d_nonClientProperty;
    static  WindowProperties::TextParsingEnabled d_textParsingEnabledProperty;
    static  WindowProperties::Margin d_marginProperty;
    static  WindowProperties::UpdateMode d_updateModeProperty;
    static  WindowProperties::MouseInputPropagationEnabled d_mouseInputPropagationProperty;

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! definition of type used for the list of attached child windows.
    typedef std::vector<Window*> ChildList;
    //! definition of type used for the UserString dictionary.
    typedef std::map<String, String, String::FastLessCompare> UserStringMap;
    //! definition of type used to track properties banned from writing XML.
    typedef std::set<String, String::FastLessCompare> BannedXMLPropertySet;

    //! type of Window (also the name of the WindowFactory that created us)
    const String d_type;
    //! The name of the window (GUI system unique).
    String d_name;
    //! Type name of the window as defined in a Falagard mapping.
    String d_falagardType;
    //! true when this window is an auto-window (it's name contains __auto_)
    bool d_autoWindow;

    //! true when this window is currently being initialised (creating children etc)
    bool d_initialising;
    //! true when this window is being destroyed.
    bool d_destructionStarted;
    //! true when Window is enabled
    bool d_enabled;
    //! is window visible (i.e. it will be rendered, but may still be obscured)
    bool d_visible;
    //! true when Window is the active Window (receiving inputs).
    bool d_active;

    //! The list of child Window objects attached to this.
    ChildList d_children;
    //! Child window objects arranged in rendering order.
    ChildList d_drawList;
    //! Holds pointer to the parent window.
    Window* d_parent;
    //! true when Window will be auto-destroyed by parent.
    bool d_destroyedByParent;

    //! true when Window will be clipped by parent Window area Rect.
    bool d_clippedByParent;
    //! true if Window is in non-client (outside InnerRect) area of parent.
    bool d_nonClientContent;

    //! Name of the Look assigned to this window (if any).
    String d_lookName;
    //! The WindowRenderer module that implements the Look'N'Feel specification
    WindowRenderer* d_windowRenderer;
    //! Object which acts as a cache of geometry drawn by this Window.
    GeometryBuffer* d_geometry;
    //! RenderingSurface owned by this window (may be 0)
    RenderingSurface* d_surface;
    //! true if window geometry cache needs to be regenerated.
    mutable bool d_needsRedraw;
    //! holds setting for automatic creation of of surface (RenderingWindow)
    bool d_autoRenderingWindow;

    //! Holds pointer to the Window objects current mouse cursor image.
    const Image* d_mouseCursor;

    //! Alpha transparency setting for the Window
    float d_alpha;
    //! true if the Window inherits alpha from the parent Window
    bool d_inheritsAlpha;

    //! Window that has captured inputs
    static Window* d_captureWindow;
    //! The Window that previously had capture (used for restoreOldCapture mode)
    Window* d_oldCapture;
    //! Restore capture to the previous capture window when releasing capture.
    bool d_restoreOldCapture;
    //! Whether to distribute captured inputs to child windows.
    bool d_distCapturedInputs;

    //! Holds pointer to the Window objects current Font.
    Font* d_font;
    //! Holds the text / label / caption for this Window.
    String d_textLogical;
    //! pointer to bidirection support object
    BiDiVisualMapping* d_bidiVisualMapping;
    //! whether bidi visual mapping has been updated since last text change.
    mutable bool d_bidiDataValid;
    //! RenderedString representation of text string as ouput from a parser.
    mutable RenderedString d_renderedString;
    //! true if d_renderedString is valid, false if needs re-parse.
    mutable bool d_renderedStringValid;
    //! Shared instance of a parser to be used in most instances.
    static BasicRenderedStringParser d_basicStringParser;
    //! Shared instance of a parser to be used when rendering text verbatim.
    static DefaultRenderedStringParser d_defaultStringParser;
    //! Pointer to a custom (user assigned) RenderedStringParser object.
    RenderedStringParser* d_customStringParser;
    //! true if use of parser other than d_defaultStringParser is enabled
    bool d_textParsingEnabled;

	//! Margin, only used when the Window is inside LayoutContainer class
    UBox d_margin;

    //! User ID assigned to this Window
    uint d_ID;
    //! Holds pointer to some user assigned data.
    void* d_userData;
    //! Holds a collection of named user string values.
    UserStringMap d_userStrings;

    //! true if Window will be drawn on top of all other Windows
    bool d_alwaysOnTop;
    //! whether window should rise in the z order when left clicked.
    bool d_riseOnClick;
    //! true if the Window responds to z-order change requests.
    bool d_zOrderingEnabled;

    //! true if the Window wishes to hear about multi-click mouse events.
    bool d_wantsMultiClicks;
    //! whether (most) mouse events pass through this window
    bool d_mousePassThroughEnabled;
    //! whether pressed mouse button will auto-repeat the down event.
    bool d_autoRepeat;
    //! seconds before first repeat event is fired
    float d_repeatDelay;
    //! secons between further repeats after delay has expired.
    float d_repeatRate;
    //! button we're tracking for auto-repeat purposes.
    MouseButton d_repeatButton;
    //! implements repeating - is true after delay has elapsed,
    bool d_repeating;
    //! implements repeating - tracks time elapsed.
    float d_repeatElapsed;

    //! true if window will receive drag and drop related notifications
    bool d_dragDropTarget;

    //! Text string used as tip for this window.
    String d_tooltipText;
    //! Possible custom Tooltip for this window.
    Tooltip* d_customTip;
    //! true if this Window created the custom Tooltip.
    bool d_weOwnTip;
    //! whether tooltip text may be inherited from parent.
    bool d_inheritsTipText;

    //! true if this window is allowed to write XML, false if not
    bool d_allowWriteXML;
    //! collection of properties not to be written to XML for this window.
    BannedXMLPropertySet d_bannedXMLProperties;

    //! This Window objects area as defined by a URect.
    URect d_area;
    //! Current constrained pixel size of the window.
    Size d_pixelSize;
    //! current minimum size for the window.
    UVector2 d_minSize;
    //! current maximum size for the window.
    UVector2 d_maxSize;
    //! Specifies the base for horizontal alignment.
    HorizontalAlignment d_horzAlign;
    //! Specifies the base for vertical alignment.
    VerticalAlignment d_vertAlign;
    //! Rotation angles for this window
    Vector3 d_rotation;

    //! outer area rect in screen pixels
    mutable Rect d_outerUnclippedRect;
    //! inner area rect in screen pixels
    mutable Rect d_innerUnclippedRect;
    //! outer area clipping rect in screen pixels
    mutable Rect d_outerRectClipper;
    //! inner area clipping rect in screen pixels
    mutable Rect d_innerRectClipper;
    //! area rect used for hit-testing agains this window
    mutable Rect d_hitTestRect;

    mutable bool d_outerUnclippedRectValid;
    mutable bool d_innerUnclippedRectValid;
    mutable bool d_outerRectClipperValid;
    mutable bool d_innerRectClipperValid;
    mutable bool d_hitTestRectValid;

    //! The mode to use for calling Window::update
    WindowUpdateMode d_updateMode;
    //! specifies whether mouse inputs should be propagated to parent(s)
    bool d_propagateMouseInputs;


private:
    /*************************************************************************
        May not copy or assign Window objects
    *************************************************************************/
    Window(const Window&) : PropertySet(), EventSet() {}
    Window& operator=(const Window&) {return *this;}
};

}
