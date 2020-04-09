/* *******************************************************************************/
/*                                 GRISBI                                        */
/*              Programme de gestion financière personnelle                      */
/*                              license : GPLv2                                  */
/*                                                                               */
/*     Copyright (C)    2000-2008 Cédric Auger (cedric@grisbi.org)               */
/*                      2003-2008 Benjamin Drieu (bdrieu@april.org)              */
/*          2008-2020 Pierre Biava (grisbi@pierre.biava.name)                    */
/*          http://www.grisbi.org                                                */
/*                                                                               */
/*     This program is free software; you can redistribute it and/or modify      */
/*     it under the terms of the GNU General Public License as published by      */
/*     the Free Software Foundation; either version 2 of the License, or         */
/*     (at your option) any later version.                                       */
/*                                                                               */
/*     This program is distributed in the hope that it will be useful,           */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*     GNU General Public License for more details.                              */
/*                                                                               */
/*     You should have received a copy of the GNU General Public License         */
/*     along with this program; if not, write to the Free Software               */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/*                                                                               */
/* *******************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>

/*START_INCLUDE*/
#include "prefs_page_fyear.h"
#include "gsb_file.h"
#include "structures.h"
#include "utils_prefs.h"
#include "erreur.h"

/*END_INCLUDE*/

/*START_EXTERN*/
/*END_EXTERN*/

typedef struct _PrefsPageFyearPrivate   PrefsPageFyearPrivate;

struct _PrefsPageFyearPrivate
{
	GtkWidget *			vbox_fyear;

    //~ GtkWidget *			checkbutton_;
	GtkWidget *			sw_fyear;
    //~ GtkWidget *         spinbutton_n;
    //~ GtkWidget *         filechooserbutton_;

};

G_DEFINE_TYPE_WITH_PRIVATE (PrefsPageFyear, prefs_page_fyear, GTK_TYPE_BOX)

/******************************************************************************/
/* Private functions                                                          */
/******************************************************************************/
/**
 * Création de la page de gestion des fyear
 *
 * \param prefs
 *
 * \return
 **/
static void prefs_page_fyear_setup_page (PrefsPageFyear *page)
{
	GtkWidget *head_page;
	gboolean is_loading;
	PrefsPageFyearPrivate *priv;

	devel_debug (NULL);

	priv = prefs_page_fyear_get_instance_private (page);
	is_loading = grisbi_win_file_is_loading ();

	/* On récupère le nom de la page */
	head_page = utils_prefs_head_page_new_with_title_and_icon (_("Financial years"), "gsb-financial-years-32.png");
	gtk_box_pack_start (GTK_BOX (priv->vbox_fyear), head_page, FALSE, FALSE, 0);
	gtk_box_reorder_child (GTK_BOX (priv->vbox_fyear), head_page, 0);

    /* set the variables for account */
    //~ gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (priv->checkbutton_),
								  //~ conf.);
	/* set spinbutton value */
	//~ gtk_spin_button_set_value (GTK_SPIN_BUTTON (priv->spinbutton_),
			
	if (!is_loading)
	{
		//~ gtk_widget_set_sensitive (priv->grid_display_modes, FALSE);

	}
				   //~ conf.);

    /* Connect signal */
    //~ g_signal_connect (priv->eventbox_,
					  //~ "button-press-event",
					  //~ G_CALLBACK (utils_prefs_page_eventbox_clicked),
					  //~ priv->checkbutton_);

    //~ g_signal_connect (priv->checkbutton_,
					  //~ "toggled",
					  //~ G_CALLBACK (utils_prefs_page_checkbutton_changed),
					  //~ &conf.);

    /* callback for spinbutton_ */
    //~ g_object_set_data (G_OBJECT (priv->spinbutton_),
                       //~ "button", priv->checkbutton_);
	//~ g_object_set_data (G_OBJECT (priv->checkbutton_),
                       //~ "spinbutton", priv->spinbutton_);

    //~ g_signal_connect (priv->spinbutton_,
					  //~ "value-changed",
					  //~ G_CALLBACK (utils_prefs_spinbutton_changed),
					  //~ &conf.);

    /* connect the signal for filechooserbutton_backup */
    //~ g_signal_connect (G_OBJECT (priv->filechooserbutton_backup),
                      //~ "selection-changed",
                      //~ G_CALLBACK (utils_prefs_page_dir_chosen),
                      //~ "backup_path");
}

/******************************************************************************/
/* Fonctions propres à l'initialisation des fenêtres                          */
/******************************************************************************/
static void prefs_page_fyear_init (PrefsPageFyear *page)
{
	gtk_widget_init_template (GTK_WIDGET (page));

	prefs_page_fyear_setup_page (page);
}

static void prefs_page_fyear_dispose (GObject *object)
{
	G_OBJECT_CLASS (prefs_page_fyear_parent_class)->dispose (object);
}

static void prefs_page_fyear_class_init (PrefsPageFyearClass *klass)
{
	G_OBJECT_CLASS (klass)->dispose = prefs_page_fyear_dispose;

	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
												 "/org/gtk/grisbi/ui/prefs_page_fyear.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsPageFyear, vbox_fyear);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsPageFyear, sw_fyear);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsPageFyear, spinbutton_);
	//~ gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), PrefsPageFyear, filechooserbutton_);
}

/******************************************************************************/
/* Public functions                                                           */
/******************************************************************************/
/**
 *
 *
 * \param
 *
 * \return
 **/
PrefsPageFyear *prefs_page_fyear_new (GrisbiPrefs *win)
{
  return g_object_new (PREFS_PAGE_FYEAR_TYPE, NULL);
}

/**
 *
 *
 * \param
 *
 * \return
 **/
/* Local Variables: */
/* c-basic-offset: 4 */
/* End: */
