/*!
* @file transferfunctionwidget.h
* @author Vladimir Molchanov (original code)
* @author Anatoliy Antonov (refactoring, commenting)
*
* @brief The widget for editing a collection of transfer functions (wrapper for TransferFunctionEditorWidget)
*/

#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QWidget>
#include "ui_transferfunctionwidget.h"
#include "transferfunctionobject.h"

namespace VCGL{
	struct IColorizer;
	class TransferFunctionEditor;
}

class TransferFunctionEditorWidget;

/// The widget for editing a collection of transfer functions (wrapper for TransferFunctionEditorWidget)
class TransferFunctionWidget : public QWidget
{
    Q_OBJECT
    
public:
    /// Default constructor
    TransferFunctionWidget(QWidget *parent = 0);
    /// Destructor
    ~TransferFunctionWidget();

    /// Initialize a widget from a collection of transfer functions
    void initialize(const std::vector<VCGL::TransferFunctionObject>& transferFunctionCollection = std::vector<VCGL::TransferFunctionObject>());

    /*! @brief Initialize a widget from a transfer function editor logic
     *
     * @param pTransferFunctionEditor Object manipulating the logic of the editing process
     * @param bTakeOwnership 	true if the widget is responsible for deleting the editor logic afterward,
     * 							false if the editor logic is managed elsewhere
     */
    void initialize(VCGL::TransferFunctionEditor* pTransferFunctionEditor, bool bTakeOwnership = false);

    /// Set range minimum and maximum (for the legend)
    void setColorDataRange(float colorDataMin = 0.0f, float colorDataMax = 1.0);
    /// Find a color for the specified data value in the selected transfer function
    QColor getColor(const float dataValue) const;

    /// Read access the current transfer function (no copying)
    const VCGL::IColorizer* getColorizer() const;

    /*! @brief Clones the transfer function object
	 * @attention Remember to call DELETE on the returned pointer!!!
	 *
	 * @return Clone of the currently selected transfer function
	 */
    VCGL::IColorizer* cloneColorizer() const;
    
    /// Return number of transfer functions in the collection
    unsigned getTFCount();

    /// Select the transfer function at the specified index
    void setCurrentTF(unsigned tfIndex);
private slots:
	///Called when the user selects a new transfer function in the UI
    void on_comboBox_colorScheme_currentIndexChanged(int index);
    /// @name Mode changes
    //@{
    	///Called when the state of "banded midpoint mode" radio button changes
    	void on_rbtn_BandedMidpoint_toggled(bool checked);
    	///Called when the state of "banded sides mode" radio button changes
    	void on_rbtn_BandedSides_toggled(bool checked);
    	///Called when the state of "continuous mode" radio button changes
    	void on_rbtn_Continuous_toggled(bool checked);
	//@}

private:
    /*! @brief Manage a new editor logic object
     *
     * @param pTransferFunctionEditor Provided object manipulating the logic of the editing process
     * @param bTakeOwnership	true if the widget is responsible for deleting the editor logic afterward,
     * 							false if the editor logic is managed elsewhere
     */
    void attachEditor(VCGL::TransferFunctionEditor* pTransferFunctionEditor, bool bTakeOwnership);
    /// Release the used editor logic object according to the ownership
    void releaseEditor();
    /// Initialize the widget from the editor logic object
    void initWithEditor();

    Ui::TransferFunctionWidget ui;					///< Qt user interface
    TransferFunctionEditorWidget *pTFEditorWidget;  ///< widget for transfer function representation
    VCGL::TransferFunctionEditor* pEditor;			///< the editor logic object
    bool bEditorOwnership;							///< the ownership on the editor logic object

    /// Populate the list of the transfer functions with mini-previews
    void addIcons();

signals:
	/// Signals that the transfer function has been changed
    void signal_tf_updated();
};

#endif // TRANSFERFUNCTIONWIDGET_H
