#ifndef PREFERENCEPANE_H
#define PREFERENCEPANE_H

#include "multiplatform/QDialog"
#include "ui_preferencepane.h"
#include "preferencepanelogic.h"

namespace VCGL {
	struct IColorizer;
	class TransferFunctionEditor;
}

class PreferencePane : public QDialog, private VCGL::PreferencePaneView
{
    Q_OBJECT

public:
    PreferencePane(QWidget *parent = 0);
    virtual ~PreferencePane();

    void getPreferences(VCGL::Preferences* pPreferences);
    void init(const VCGL::Preferences* pPreferences);
    virtual void updateView(const VCGL::Preferences& preferences) override;
signals:
	void dialogAccepted();
	void dialogRejected();

public slots:
	void on_btnOK_clicked();
	void on_btnCancel_clicked();
	void on_btnApply_clicked();
	void on_btnTCStartPointsColor_clicked();
	void on_btnTCEndPointsColor_clicked();
	void on_btnTCLinesColor_clicked();
	void on_btnCorrOddPointsColor_clicked();
	void on_btnCorrEvenPointsColor_clicked();
	void on_btnCorrLinesColor_clicked();
	void on_btnRefPointColor_clicked();

	void on_rbtnSSOff_clicked(bool checked);
	void on_rbtnSignificant_clicked(bool checked);
	void on_rbtnHighlySignificant_clicked(bool checked);

	void on_rbtnEquirectangular_clicked(bool checked);
	void on_rbtnPolar_clicked(bool checked);

private:
	void setColorToButton(const VCGL::RGBF& color, QAbstractButton* pButton);
	QColor getButtonColor(const QAbstractButton* pButton);
	VCGL::RGBF getNewColor(const QAbstractButton* pButton);

    Ui::PreferencePaneClass ui;
    VCGL::PreferencePaneLogic* pLogic;
    VCGL::TransferFunctionEditor* pCorrelationTFEditor;
    VCGL::TransferFunctionEditor* pTeleconnectivityTFEditor;
};

#endif // PREFERENCEPANE_H
