/*  Fichier qui s'occupe d'afficher les �tats via gnomeprint */
/*      etats_gnomeprint.c */

/*     Copyright (C) 2002  Benjamin Drieu */
/* 			bdrieu@april.org */
/* 			http://www.grisbi.org */

/*     This program is free software; you can redistribute it and/or modify */
/*     it under the terms of the GNU General Public License as published by */
/*     the Free Software Foundation; either version 2 of the License, or */
/*     (at your option) any later version. */

/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     You should have received a copy of the GNU General Public License */
/*     along with this program; if not, write to the Free Software */
/*     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "include.h"
#include "structures.h"
#include "variables-extern.c"
#include "en_tete.h"

#include "etats_gnomeprint.h"

GtkWidget * table_etat;

struct struct_etat_affichage gnomeprint_affichage = {
  gnomeprint_initialise,
  gnomeprint_affiche_titre,
  gnomeprint_affiche_separateur,
  gnomeprint_affiche_total_categories,
  gnomeprint_affiche_total_sous_categ,
  gnomeprint_affiche_total_ib,
  gnomeprint_affiche_total_sous_ib,
  gnomeprint_affiche_total_compte,
  gnomeprint_affiche_total_tiers,
  gnomeprint_affichage_ligne_ope,
  gnomeprint_affiche_total_partiel,
  gnomeprint_affiche_total_general,
  gnomeprint_affiche_categ_etat,
  gnomeprint_affiche_sous_categ_etat,
  gnomeprint_affiche_ib_etat,
  gnomeprint_affiche_sous_ib_etat,
  gnomeprint_affiche_compte_etat,
  gnomeprint_affiche_tiers_etat,
  gnomeprint_affiche_titre_revenus_etat,
  gnomeprint_affiche_titre_depenses_etat,
  gnomeprint_affiche_totaux_sous_jaccent,
  gnomeprint_affiche_titres_colonnes,
  gnomeprint_finish
};


GnomePrintContext *pc = NULL;
GnomePrintMaster *gpm = NULL;
int do_preview=0;
GnomeFont *title_font, *subtitle_font, *header_font, *text_font;
float point_x, point_y, tmp_x, tmp_y;
gfloat red=0, green=0, blue=0;



my_gnome_print_dialog_new (const char *title, int flags)
{
	GtkWidget *w;

	w = GTK_WIDGET ( gtk_type_new (gnome_print_dialog_get_type ()));
	if (GNOME_PRINT_DIALOG (w)->printer == NULL)
		return NULL;
	gnome_dialog_constructv(GNOME_DIALOG(w), title, print_buttons());
	init_body(GNOME_PRINT_DIALOG(w), flags);
	return w;
}



/*****************************************************************************************************/
/* Initialise les structures n�cessaires pour gnome-print */
/*  - gpd : bo�te de dialogue pour demander les r�glages du print master */
/*  - gpm : le gnome print master */
/*  - pc : le contexte d'impression */
/*  - diverses polices */
/*****************************************************************************************************/
gint gnomeprint_initialise ()
{
  GnomePrintDialog *gpd;
  static int copies=1, collate;

  gpd = GNOME_PRINT_DIALOG (gnome_print_dialog_new("Impression de Grisbi", 
						   GNOME_PRINT_DIALOG_COPIES));
  gnome_print_dialog_set_copies(gpd, copies, collate);

  switch (gnome_dialog_run(GNOME_DIALOG(gpd))) {
  case GNOME_PRINT_PRINT:
    do_preview = 0;
    break;
  case GNOME_PRINT_PREVIEW:
    do_preview = 1;
    break;
  case GNOME_PRINT_CANCEL:
    gnome_dialog_close (GNOME_DIALOG(gpd));
    return 0;
  }

  gpm = gnome_print_master_new();
  gnome_print_dialog_get_copies(gpd, &copies, &collate);
  gnome_print_master_set_copies(gpm, copies, collate);
  gnome_print_master_set_printer(gpm, gnome_print_dialog_get_printer(gpd));

  gnome_dialog_close (GNOME_DIALOG(gpd));
  pc = gnome_print_master_get_context(gpm);

  title_font = gnome_font_new_closest ("Utopia", GNOME_FONT_BOLD, 0, 36);
  subtitle_font = gnome_font_new_closest ("Times", GNOME_FONT_BOLD, 0, 20);
  header_font = gnome_font_new_closest ("Times", GNOME_FONT_BOLD, 0, 12);
  text_font = gnome_font_new_closest ("Times", GNOME_FONT_BOOK, 0, 12);

  tmp_x = point_x = 10;
  tmp_y = point_y = gnome_paper_psheight(gnome_print_master_get_paper(gpm));

  return 1;
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* modifie la couleur utilis�e pour l'affichage */
/*****************************************************************************************************/
void gnomeprint_set_color ( gfloat tred, gfloat tgreen, gfloat tblue )
{
  red = tred;
  green = tgreen;
  blue = tblue;
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* d�place le point de r�f�rence d'un certain nombre de millim�tres */
/*****************************************************************************************************/
void gnomeprint_move_point ( gfloat x, gfloat y )
{
  point_x = tmp_x + gnome_paper_convert_to_points(x, gnome_unit_with_name ("Millimeter"));
  point_y = tmp_y + gnome_paper_convert_to_points(y, gnome_unit_with_name ("Millimeter"));
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Met � jour le point de r�f�rence � partir du point temporaire */
/*****************************************************************************************************/
void gnomeprint_commit_point ( )
{
  gnomeprint_commit_x ( );
  gnomeprint_commit_y ( );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Met � jour le point temporaire � partir du point de r�f�rence */
/*****************************************************************************************************/
void gnomeprint_update_point ( )
{
  tmp_x = point_x;
  tmp_y = point_y;
}


/*****************************************************************************************************/
/* Met � jour la coordonn�e x du point temporaire � partir du point de r�f�rence */
/*****************************************************************************************************/
void gnomeprint_update_x ( )
{
  tmp_x = point_x;
}


/*****************************************************************************************************/
/* Met � jour la coordonn�e y du point temporaire � partir du point de r�f�rence */
/*****************************************************************************************************/
void gnomeprint_update_y ( )
{
  tmp_y = point_y;
}


/*****************************************************************************************************/
/* Met � jour la composante Y du point de r�f�rence */
/*****************************************************************************************************/
void gnomeprint_commit_x ( )
{
  point_x = tmp_x;
}


/*****************************************************************************************************/
/* Met � jour la composante Y du point de r�f�rence */
/*****************************************************************************************************/
void gnomeprint_commit_y ( )
{
  point_y = tmp_y;
}


/*****************************************************************************************************/
/* affiche le titre sur la premi�re page du rapport */
/*****************************************************************************************************/
gint gnomeprint_affiche_titre ( gint ligne )
{
  /* TODO: utiliser un compteur de page ? */
  gnome_print_beginpage (pc, "1");

  gnomeprint_set_color ( 1, 0, 0 );
  gnomeprint_affiche_texte (etat_courant -> nom_etat, title_font);
  gnomeprint_commit_point ();

  gnomeprint_set_color ( 0, 0, 0 );
/*   gnomeprint_move_point ( 0, -10 ); */

  /* FIXME: crado */
/*   p_tab_nom_de_compte_variable = p_tab_nom_de_compte; */
/*   for ( i=0 ; i < nb_comptes ; i++ ) */
/*     { */
/*       gnomeprint_affiche_texte(NOM_DU_COMPTE, subtitle_font); */
/*       gnomeprint_commit_point(); */
/*       p_tab_nom_de_compte_variable++; */
/*     } */

  return 1;
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Affiche le texte pass� en argument dans une certaine police */
/* Est sensible � la position du point de r�f�rence et � la couleur */
/* D�place le point temporaire */
/*****************************************************************************************************/
void gnomeprint_affiche_texte ( char * texte, GnomeFont * font)
{
  gint font_height = gnome_font_get_size(font);

  gnomeprint_update_point ( );

  gnome_print_gsave (pc);
  gnome_print_setfont (pc, font);
  gnome_print_setrgbcolor (pc, red, green, blue); 
  gnome_print_moveto (pc, point_x, point_y-font_height);
  gnome_print_show (pc, latin2utf8(texte));
  gnome_print_grestore (pc);

  tmp_y -= font_height;
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* Affiche un */
/*****************************************************************************************************/
void gnomeprint_barre_verticale ( gint taille )
{
  gnome_print_gsave (pc);
  gnome_print_setlinewidth (pc, 1);
  gnome_print_moveto (pc, point_x, point_y);
  gnome_print_lineto (pc, 
		      point_x, point_y + taille);
  gnome_print_stroke (pc);
  gnome_print_grestore (pc);
}


/*****************************************************************************************************/
/* Affiche un s�parateur horizontal */
/*****************************************************************************************************/
gint gnomeprint_affiche_separateur ( gint ligne )
{
  gnomeprint_move_point ( 0, -5 ); 

  gnome_print_gsave (pc);
  gnome_print_setlinewidth (pc, 2);
  gnome_print_moveto (pc, point_x, point_y);
  gnome_print_lineto (pc, 
		      gnome_paper_pswidth(gnome_print_master_get_paper(gpm))-10, 
		      point_y); 
  gnome_print_stroke (pc);
  gnome_print_grestore (pc);

  gnomeprint_move_point ( 0, -5 ); 

  return ligne + 1;
}


/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les cat�gories sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_categories ( gint ligne )
{

  montant_categ_etat = 0;
  nom_categ_en_cours = NULL;
  titres_affiches = 0;
  
  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les sous_categ sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_sous_categ ( gint ligne )
{


  montant_sous_categ_etat = 0;
  nom_ss_categ_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les ib sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_ib ( gint ligne )
{

  montant_ib_etat = 0;
  nom_ib_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les sous_ib sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_sous_ib ( gint ligne )
{

  montant_sous_ib_etat = 0;
  nom_ss_ib_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les compte sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_compte ( gint ligne )
{

  montant_compte_etat = 0;
  nom_compte_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
/* affiche le total � l'endroit donn� de la table */
/* si les tiers sont affich�es */
/* retourne le ligne suivante de la table */
/*****************************************************************************************************/
gint gnomeprint_affiche_total_tiers ( gint ligne )
{

  montant_tiers_etat = 0;
  nom_tiers_en_cours = NULL;
  titres_affiches = 0;

  return (ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gnomeprint_affichage_ligne_ope ( struct structure_operation *operation,
			   gint ligne )
{
  int colonne;
  if ( etat_courant -> afficher_opes )
    {
      /* on affiche ce qui est demand� pour les op�s */


      /* si les titres ne sont pas affich�s et qu'il faut le faire, c'est ici */

      if ( !titres_affiches
	   &&
	   etat_courant -> afficher_titre_colonnes
	   &&
	   etat_courant -> type_affichage_titres )
	ligne = gnomeprint_affichage . affiche_titres_colonnes ( ligne );

      colonne = 1;

      if ( etat_courant -> afficher_no_ope )
	{
	  int size;
	  gnomeprint_affiche_texte (itoa ( operation -> no_operation ), text_font);
	  size = tmp_y - point_y;
	  gnomeprint_update_point ();
	  gnomeprint_move_point ( 8, 0 );
	  gnomeprint_update_point ( );
	  gnomeprint_barre_verticale ( size );
	  gnomeprint_move_point ( 2, 0 );
	  gnomeprint_update_point ();
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_date_ope )
	{
	  int size;
	   gnomeprint_affiche_texte ( g_strdup_printf  ( "%.2d/%.2d/%d",
							 operation -> jour,
							 operation -> mois,
							 operation -> annee ),
				      text_font);
	   size = tmp_y - point_y;
	   gnomeprint_update_point ();
	   gnomeprint_move_point ( 20, 0 );
	   gnomeprint_update_point ( );
	   gnomeprint_barre_verticale ( size );
	   gnomeprint_move_point ( 2, 0 );
	   gnomeprint_update_point ();
	   colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_exo_ope )
	{
	  if ( operation -> no_exercice )
	    {
	      int size;
	      gnomeprint_affiche_texte( ((struct struct_exercice *)(g_slist_find_custom ( liste_struct_exercices,
											  GINT_TO_POINTER ( operation -> no_exercice ),
											  (GCompareFunc) recherche_exercice_par_no )->data)) -> nom_exercice, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point ();
	      gnomeprint_move_point ( 70, 0 );
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_tiers_ope )
	{
	  int size;
	  if ( operation -> tiers )
	    {
	      gnomeprint_affiche_texte( ((struct struct_tiers *)
					 (g_slist_find_custom ( liste_struct_tiers,
								GINT_TO_POINTER ( operation -> tiers ),
								(GCompareFunc) recherche_tiers_par_no )->data)) -> nom_tiers , 
					text_font);
	    }
	  size = tmp_y - point_y;
	  gnomeprint_update_point();
	  gnomeprint_move_point ( 80, 0);
	  gnomeprint_update_point ( );
	  gnomeprint_barre_verticale ( size );
	  gnomeprint_move_point ( 2, 0 );
	  gnomeprint_update_point ();
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_categ_ope )
	{
	  gchar *pointeur;

	  pointeur = NULL;

	  if ( operation -> categorie )
	    {
	      struct struct_categ *categ;

	      categ = g_slist_find_custom ( liste_struct_categories,
					    GINT_TO_POINTER ( operation -> categorie ),
					    (GCompareFunc) recherche_categorie_par_no ) -> data;
	      pointeur = categ -> nom_categ;

	      if ( operation -> sous_categorie
		   &&
		   etat_courant -> afficher_sous_categ_ope )
		pointeur = g_strconcat ( pointeur,
					 " : ",
					 ((struct struct_sous_categ *)(g_slist_find_custom ( categ -> liste_sous_categ,
											     GINT_TO_POINTER ( operation -> sous_categorie ),
											     (GCompareFunc) recherche_sous_categorie_par_no ) -> data )) -> nom_sous_categ,
					 NULL );
	    }
	  else
	    {
	      /* si c'est un virement, on le marque, sinon c'est qu'il n'y a pas de categ */
	      /* ou que c'est une op� ventil�e, et on marque rien */

	      if ( operation -> relation_no_operation )
		{
		  /* c'est un virement */

		  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> relation_no_compte;

		  if ( operation -> montant < 0 )
		    pointeur = g_strconcat ( "Virement vers ",
					     NOM_DU_COMPTE,
					     NULL );
		  else
		    pointeur = g_strconcat ( "Virement de ",
					     NOM_DU_COMPTE,
					     NULL );
		}
	    }

	  if ( pointeur )
	    {
	      int size;
	      gnomeprint_affiche_texte ( pointeur, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}



      if ( etat_courant -> afficher_ib_ope )
	{
	  if ( operation -> imputation )
	    {
	      struct struct_imputation *ib;
	      int size;
	      gchar *pointeur;

	      ib = g_slist_find_custom ( liste_struct_imputation,
					 GINT_TO_POINTER ( operation -> imputation ),
					 (GCompareFunc) recherche_imputation_par_no ) -> data;
	      pointeur = ib -> nom_imputation;

	      if ( operation -> sous_imputation
		   &&
		   etat_courant -> afficher_sous_ib_ope )
		pointeur = g_strconcat ( pointeur,
					 " : ",
					 ((struct struct_sous_imputation *)(g_slist_find_custom ( ib -> liste_sous_imputation,
												  GINT_TO_POINTER ( operation -> sous_imputation ),
												  (GCompareFunc) recherche_sous_imputation_par_no ) -> data )) -> nom_sous_imputation,
					 NULL );

	      gnomeprint_affiche_texte ( pointeur, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_notes_ope )
	{
	  if ( operation -> notes )
	    {
	      int size;
	      gnomeprint_affiche_texte ( operation -> notes, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_type_ope )
	{
	  GSList *pointeur;

	  p_tab_nom_de_compte_variable = p_tab_nom_de_compte + operation -> no_compte;

	  pointeur = g_slist_find_custom ( TYPES_OPES,
					   GINT_TO_POINTER ( operation -> type_ope ),
					   (GCompareFunc) recherche_type_ope_par_no );

	  if ( pointeur )
	    {
	      struct struct_type_ope *type;
	      int size;

	      type = pointeur -> data;

	      gnomeprint_affiche_texte ( type -> nom_type, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_cheque_ope )
	{
	  if ( operation -> contenu_type )
	    {
	      int size;
	      gnomeprint_affiche_texte ( operation -> contenu_type, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}


      if ( etat_courant -> afficher_pc_ope )
	{
	  if ( operation -> no_piece_comptable )
	    {
	      int size;
	      gnomeprint_affiche_texte ( operation -> no_piece_comptable, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_infobd_ope )
	{
	  if ( operation -> info_banque_guichet )
	    {
	      int size;
	      gnomeprint_affiche_texte ( operation -> info_banque_guichet, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}

      if ( etat_courant -> afficher_rappr_ope )
	{
	  GSList *pointeur;

	  pointeur = g_slist_find_custom ( liste_no_rapprochements,
					   GINT_TO_POINTER ( operation -> no_rapprochement ),
					   (GCompareFunc) recherche_no_rapprochement_par_no );

	  if ( pointeur )
	    {
	      struct struct_no_rapprochement *rapprochement;
	      int size;

	      rapprochement = pointeur -> data;
	      gnomeprint_affiche_texte ( rapprochement -> nom_rapprochement, text_font );
	      size = tmp_y - point_y;
	      gnomeprint_update_point();
	      gnomeprint_move_point ( 70, 0);
	      gnomeprint_update_point ( );
	      gnomeprint_barre_verticale ( size );
	      gnomeprint_move_point ( 2, 0 );
	      gnomeprint_update_point ();
	    }
	  colonne = colonne + 2;
	}


      /* on affiche le montant au bout de la ligne */

      if ( devise_compte_en_cours_etat
	   &&
	   operation -> devise == devise_compte_en_cours_etat -> no_devise )
	gnomeprint_affiche_texte ( g_strdup_printf  ("%4.2f %s",
						     operation -> montant,
						     devise_compte_en_cours_etat -> code_devise ), text_font);
      else
	{
	  struct struct_devise *devise_operation;

	  devise_operation = g_slist_find_custom ( liste_struct_devises,
						   GINT_TO_POINTER ( operation -> devise ),
						   ( GCompareFunc ) recherche_devise_par_no ) -> data;
	  gnomeprint_affiche_texte ( g_strdup_printf  ("%4.2f %s",
						    operation -> montant,
						    devise_operation -> code_devise ), text_font);
	}

      if ( ligne_debut_partie == -1 )
	ligne_debut_partie = ligne;

      ligne++;
    }

  tmp_x = 10;
  gnomeprint_commit_point ();

  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gnomeprint_affiche_total_partiel ( gdouble total_partie,
			     gint ligne,
			     gint type )
{

  return ( ligne );
}
/*****************************************************************************************************/

/*****************************************************************************************************/
gint gnomeprint_affiche_total_general ( gdouble total_general,
			     gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gnomeprint_affiche_categ_etat ( struct structure_operation *operation,
			  			  gchar *decalage_categ,
			  gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gnomeprint_affiche_sous_categ_etat ( struct structure_operation *operation,
			       			       gchar *decalage_sous_categ,
			       gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gnomeprint_affiche_ib_etat ( struct structure_operation *operation,
		       		       gchar *decalage_ib,
		       gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gnomeprint_affiche_sous_ib_etat ( struct structure_operation *operation,
			    			    gchar *decalage_sous_ib,
			    gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gnomeprint_affiche_compte_etat ( struct structure_operation *operation,
			   			   gchar *decalage_compte,
			   gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/




/*****************************************************************************************************/
gint gnomeprint_affiche_tiers_etat ( struct structure_operation *operation,
			  			  gchar *decalage_tiers,
			  gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gnomeprint_affiche_titre_revenus_etat ( gint ligne )
{
  gnomeprint_affiche_texte ( "Revenus", subtitle_font );
  gnomeprint_commit_y ( );
  gnomeprint_move_point ( 0, -5 );

  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
gint gnomeprint_affiche_titre_depenses_etat ( gint ligne )
{
  gnomeprint_affiche_texte ( "D�penses", subtitle_font );
  gnomeprint_commit_y ( );
  gnomeprint_move_point ( 0, -5 );

  return ( ligne );
}
/*****************************************************************************************************/


/*****************************************************************************************************/
/* appel�e lors de l'affichage d'une structure ( cat�g, ib ... ) */
/* affiche le total de toutes les structures sous jaccentes */
/*****************************************************************************************************/

gint gnomeprint_affiche_totaux_sous_jaccent ( gint origine,
				   gint ligne )
{

  return ( ligne );
}
/*****************************************************************************************************/



/*****************************************************************************************************/
gint gnomeprint_affiche_titres_colonnes ( gint ligne )
{
  gint colonne = 1, size = 0;

  if ( etat_courant -> afficher_no_ope )
    {
      gnomeprint_affiche_texte ( "N�", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 8, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_date_ope )
    {
      gnomeprint_affiche_texte ( "Date", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 20, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_exo_ope )
    {
      gnomeprint_affiche_texte ( "Exercice", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 30, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_tiers_ope )
    {
      gnomeprint_affiche_texte ( "Tiers", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 80, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_categ_ope )
    {
      gnomeprint_affiche_texte ( "Cat�gorie", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 70, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_ib_ope )
    {
      gnomeprint_affiche_texte ( "Imputation budg�taire", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 50, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_notes_ope )
    {
      gnomeprint_affiche_texte ( "Notes", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 50, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_type_ope )
    {
      gnomeprint_affiche_texte ( "Type", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 30, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_cheque_ope )
    {
      gnomeprint_affiche_texte ( "Ch�que", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 20, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_pc_ope )
    {
      gnomeprint_affiche_texte ( "Pi�ce comptable", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 30, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_infobd_ope )
    {
      gnomeprint_affiche_texte ( "Info banque/guichet", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 30, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  if ( etat_courant -> afficher_rappr_ope )
    {
      gnomeprint_affiche_texte ( "Relev�", header_font );
      size = tmp_y - point_y;
      gnomeprint_update_point();
      gnomeprint_move_point ( 30, 0);
      gnomeprint_update_point ( );
      gnomeprint_barre_verticale ( size );
      gnomeprint_move_point ( 2, 0 );
      gnomeprint_update_point ();

      colonne = colonne + 2;
    }

  gnomeprint_affiche_texte ( "Montant", header_font);
  gnomeprint_update_point ( );

  ligne++;

  tmp_x = 10;
  tmp_y += size;
  gnomeprint_commit_point ( );

  ligne++;

/*   gnome_print_gsave (pc); */
/*   gnome_print_setlinewidth (pc, 1); */
/*   gnome_print_moveto (pc, point_x, point_y); */
/*   gnome_print_lineto (pc,  */
/* 		      gnome_paper_pswidth(gnome_print_master_get_paper(gpm))-10,  */
/* 		      point_y);  */
/*   gnome_print_stroke (pc); */
/*   gnome_print_grestore (pc); */
/*   gnomeprint_move_point ( 0, -1 ); */

  titres_affiches = 1;

  return ( ligne );
}
/*****************************************************************************************************/


gint gnomeprint_finish ( )
{
  gnome_print_showpage (pc);

  gnome_print_master_close(gpm);
  if (do_preview)
    {
      GnomePrintMasterPreview * pmp;
      pmp = gnome_print_master_preview_new(gpm, 
					   "Pr�visualisation de l'impression de Grisbi");
      gtk_widget_show(GTK_WIDGET(pmp));
    }
  else
    {
      gnome_print_master_print(gpm);
    }

  return 1;

  // FIXME: quand est-ce qu'on le d�truit ?
  //   gtk_object_unref (GTK_OBJECT (pc));
}
