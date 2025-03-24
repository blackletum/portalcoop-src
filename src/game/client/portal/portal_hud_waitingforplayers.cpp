#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "vgui_entitypanel.h"
#include "iclientmode.h"
#include "vgui/ILocalize.h"
#include "portal_shareddefs.h"

#include "portal_gamerules.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CWaitingForPlayersText : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CWaitingForPlayersText, vgui::Panel );

public:
	CWaitingForPlayersText( const char *pElementName );
	bool ShouldDraw( void ) OVERRIDE;
	virtual void	ApplySchemeSettings( vgui::IScheme *scheme );
	virtual void	Paint( void );

private:

	vgui::HFont		m_hFont;
};

DECLARE_HUDELEMENT( CWaitingForPlayersText );

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CWaitingForPlayersText::CWaitingForPlayersText( const char *pElementName ) :
	CHudElement( pElementName ), BaseClass( NULL, "WaitingForPlayersText" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_hFont = g_hFontTrebuchet24;
}

bool CWaitingForPlayersText::ShouldDraw( void )
{
	if ( !pcoop_require_all_players.GetBool() )
		return false;

	if ( !PortalGameRules() )
		return false;
	
	if ( !PortalGameRules()->ShouldPauseGame() )
		return false;

	return CHudElement::ShouldDraw();
}

void CWaitingForPlayersText::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	m_hFont = scheme->GetFont( "TargetID", IsProportional() );

	SetPaintBackgroundEnabled( false );
}

//-----------------------------------------------------------------------------
// Purpose: Draw function for the element
//-----------------------------------------------------------------------------
void CWaitingForPlayersText::Paint()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;
	
	const char *printFormatString;
	int ypos;
	int xpos;

	if ( pPlayer->IsObserver() )
	{
		printFormatString = "#portalcoop_waiting_for_players_spectator";
		ypos = YRES(260);
		xpos = XRES(250);
	}
	else
	{
		printFormatString = "#portalcoop_waiting_for_players";
		ypos = YRES(260);
		xpos = XRES(250);
	}

	wchar_t wszFinalText[64];
	g_pVGuiLocalize->ConstructString( wszFinalText, sizeof(wszFinalText), g_pVGuiLocalize->Find(printFormatString), 0 );

	if ( wszFinalText[0] )
	{
		int wide, tall;
		vgui::surface()->GetTextSize( m_hFont, wszFinalText, wide, tall );
					
		vgui::surface()->DrawSetTextFont( m_hFont );
		vgui::surface()->DrawSetTextPos( xpos, ypos );
		
		Color c = Color( 255, 160, 32, 255 );

		vgui::surface()->DrawSetTextColor( c );
		vgui::surface()->DrawPrintText( wszFinalText, wcslen(wszFinalText) );
	}
}
