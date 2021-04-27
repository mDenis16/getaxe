#include "../includes.h"

namespace ui {

    ImVec2 operator-( ImVec2 p1, ImVec2 p2 ) { return ImVec2 ( p1.x - p2.x, p1.y - p2.y ); }
	ImVec2 operator+( ImVec2 p1, ImVec2 p2 ) { return ImVec2 ( p1.x + p2.x, p1.y + p2.y ); }
	ImVec2 operator*( ImVec2 p1, int value ) { return ImVec2 ( p1.x * value, p1.y * value ); }
    ImVec4 operator+( float val, ImVec4 p2 ) { return ImVec4 ( val + p2.x, val + p2.y, val + p2.z, val + p2.w ); }
    ImVec4 operator*( float val, ImVec4 p2 ) { return ImVec4 ( val * p2.x, val * p2.y, val * p2.z, val * p2.w ); }
    ImVec4 operator*( ImVec4 p2, float val ) { return ImVec4 ( val * p2.x, val * p2.y, val * p2.z, val * p2.w ); }
    ImVec4 operator/( ImVec4 p1, ImVec4 p2 ) { return ImVec4 ( p1.x / p2.x, p1.y / p2.y, p1.z / p2.z, p1.w / p2.w ); }
    ImVec4 operator*( ImVec4 p1, ImVec4 p2 ) { return ImVec4 ( p1.x * p2.x, p1.y * p2.y, p1.z * p2.z, p1.w * p2.w ); }
    ImVec4 operator-( ImVec4 p1, ImVec4 p2 ) { return ImVec4 ( p1.x - p2.x, p1.y - p2.y, p1.z - p2.z, p1.w - p2.w ); }
    //*shadow math*/
    ImVec4 boxGaussianIntegral ( ImVec4 x ) {
        ImVec4 s = ImVec4 ( x.x > 0 ? 1.0f : -1.0f, x.y > 0 ? 1.0f : -1.0f, x.z > 0 ? 1.0f : -1.0f, x.w > 0 ? 1.0f : -1.0f );
        ImVec4 a = ImVec4 ( fabsf ( x.x ), fabsf ( x.y ), fabsf ( x.z ), fabsf ( x.w ) );
        ImVec4 res = 1.0f + ( 0.278393f + ( 0.230389f + 0.078108f * ( a * a ) ) * a ) * a;
        ImVec4 resSquared = res * res;
        return s - s / ( resSquared * resSquared );
    }

    ImVec4 boxLinearInterpolation ( ImVec4 x ) {
        const float maxClamp = 1.0f;
        const float minClamp = -1.0f;
        return ImVec4 ( x.x > maxClamp ? maxClamp : x.x < minClamp ? minClamp : x.x,
            x.y > maxClamp ? maxClamp : x.y < minClamp ? minClamp : x.y,
            x.z > maxClamp ? maxClamp : x.z < minClamp ? minClamp : x.z,
            x.w > maxClamp ? maxClamp : x.w < minClamp ? minClamp : x.w );
    }

    float boxShadow ( ImVec2 lower, ImVec2 upper, ImVec2 point, float sigma, bool linearInterpolation ) {
        ImVec2 pointLower = point - lower;
        ImVec2 pointUpper = point - upper;
        ImVec4 query = ImVec4 ( pointLower.x, pointLower.y, pointUpper.x, pointUpper.y );
        ImVec4 pointToSample = query * ( sqrtf ( 0.5f ) / sigma );
        ImVec4 integral = linearInterpolation ? 0.5f + 0.5f * boxLinearInterpolation ( pointToSample ) : 0.5f + 0.5f * boxGaussianIntegral ( pointToSample );
        return ( integral.z - integral.x ) * ( integral.w - integral.y );
    }
    /**/
   /* void  addtempshadow ( ImDrawList * drawList, RectangleShadowSettings & settings ) {
        settings.rectSize = ImVec2 ( settings.maxs.x - settings.mins.x, settings.maxs.y - settings.mins.y );

        const int    samplesSpan = settings.samplesPerCornerSide * settings.spacingBetweenSamples;
        const int    halfWidth = static_cast< int >( settings.rectSize.x / 2 );
        const int    numSamplesInHalfWidth = ( halfWidth / settings.spacingBetweenSamples ) == 0 ? 1 : halfWidth / settings.spacingBetweenSamples;
        const int    numSamplesWidth = samplesSpan > halfWidth ? numSamplesInHalfWidth : settings.samplesPerCornerSide;
        const int    halfHeight = static_cast< int >( settings.rectSize.y / 2 );
        const int    numSamplesInHalfHeight = ( halfHeight / settings.spacingBetweenSamples ) == 0 ? 1 : halfHeight / settings.spacingBetweenSamples;
        const int    numSamplesHeight = samplesSpan > halfHeight ? numSamplesInHalfHeight : settings.samplesPerCornerSide;
        const int    numVerticesInARing = numSamplesWidth * 4 + numSamplesHeight * 4 + 4;
        const ImVec2 whiteTexelUV = ImGui::GetIO ( ).Fonts->TexUvWhitePixel;
        const ImVec2 origin = settings.mins;
        const ImVec2 rectangleTopLeft = origin;
        const ImVec2 rectangleBottomRight = rectangleTopLeft + settings.rectSize;
        const ImVec2 rectangleTopRight = rectangleTopLeft + ImVec2 ( settings.rectSize.x, 0 );
        const ImVec2 rectangleBottomLeft = rectangleTopLeft + ImVec2 ( 0, settings.rectSize.y );

        ImColor shadowColor = settings.shadowColor;
        settings.totalVertices = numVerticesInARing * settings.rings;
        settings.totalIndices = 6 * ( numVerticesInARing ) * ( settings.rings - 1 );

        drawList->PrimReserve ( settings.totalIndices, settings.totalVertices );
        const ImDrawVert * shadowVertices = drawList->_VtxWritePtr;
        ImDrawVert * vertexPointer = drawList->_VtxWritePtr;

        for ( int r = 0; r < settings.rings; ++r ) {
            const float  adaptiveScale = ( r / 2.5f ) + 1;
            const ImVec2 ringOffset = ImVec2 ( adaptiveScale * r * settings.spacingBetweenRings, adaptiveScale * r * settings.spacingBetweenRings );
            for ( int j = 0; j < 4; ++j ) {
                ImVec2      corner;
                ImVec2      direction [ 2 ];
                const float spacingBetweenSamplesOnARing = static_cast< float >( settings.spacingBetweenSamples );
                switch ( j ) {
                case 0:
                    corner = rectangleTopLeft + ImVec2 ( -ringOffset.x, -ringOffset.y );
                    direction [ 0 ] = ImVec2 ( 1, 0 ) * spacingBetweenSamplesOnARing;
                    direction [ 1 ] = ImVec2 ( 0, 1 ) * spacingBetweenSamplesOnARing;
                    for ( int i = 0; i < numSamplesWidth; ++i ) {
                        const ImVec2 point = corner + direction [ 0 ] * ( numSamplesWidth - i );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }

                    shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, corner - settings.shadowOffset, settings.sigma, settings.linear );
                    vertexPointer->pos = corner;
                    vertexPointer->uv = whiteTexelUV;
                    vertexPointer->col = shadowColor;
                    vertexPointer++;

                    for ( int i = 0; i < numSamplesHeight; ++i ) {
                        const ImVec2 point = corner + direction [ 1 ] * ( i + 1 );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }
                    break;
                case 1:
                    corner = rectangleBottomLeft + ImVec2 ( -ringOffset.x, +ringOffset.y );
                    direction [ 0 ] = ImVec2 ( 1, 0 ) * spacingBetweenSamplesOnARing;
                    direction [ 1 ] = ImVec2 ( 0, -1 ) * spacingBetweenSamplesOnARing;
                    for ( int i = 0; i < numSamplesHeight; ++i ) {
                        const ImVec2 point = corner + direction [ 1 ] * ( numSamplesHeight - i );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }

                    shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, corner - settings.shadowOffset, settings.sigma, settings.linear );
                    vertexPointer->pos = corner;
                    vertexPointer->uv = whiteTexelUV;
                    vertexPointer->col = shadowColor;
                    vertexPointer++;

                    for ( int i = 0; i < numSamplesWidth; ++i ) {
                        const ImVec2 point = corner + direction [ 0 ] * ( i + 1 );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }
                    break;
                case 2:
                    corner = rectangleBottomRight + ImVec2 ( +ringOffset.x, +ringOffset.y );
                    direction [ 0 ] = ImVec2 ( -1, 0 ) * spacingBetweenSamplesOnARing;
                    direction [ 1 ] = ImVec2 ( 0, -1 ) * spacingBetweenSamplesOnARing;
                    for ( int i = 0; i < numSamplesWidth; ++i ) {
                        const ImVec2 point = corner + direction [ 0 ] * ( numSamplesWidth - i );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }

                    shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, corner - settings.shadowOffset, settings.sigma, settings.linear );
                    vertexPointer->pos = corner;
                    vertexPointer->uv = whiteTexelUV;
                    vertexPointer->col = shadowColor;
                    vertexPointer++;

                    for ( int i = 0; i < numSamplesHeight; ++i ) {
                        const ImVec2 point = corner + direction [ 1 ] * ( i + 1 );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }
                    break;
                case 3:
                    corner = rectangleTopRight + ImVec2 ( +ringOffset.x, -ringOffset.y );
                    direction [ 0 ] = ImVec2 ( -1, 0 ) * spacingBetweenSamplesOnARing;
                    direction [ 1 ] = ImVec2 ( 0, 1 ) * spacingBetweenSamplesOnARing;
                    for ( int i = 0; i < numSamplesHeight; ++i ) {
                        const ImVec2 point = corner + direction [ 1 ] * ( numSamplesHeight - i );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }

                    shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, corner - settings.shadowOffset, settings.sigma, settings.linear );
                    vertexPointer->pos = corner;
                    vertexPointer->uv = whiteTexelUV;
                    vertexPointer->col = shadowColor;
                    vertexPointer++;

                    for ( int i = 0; i < numSamplesWidth; ++i ) {
                        const ImVec2 point = corner + direction [ 0 ] * ( i + 1 );
                        shadowColor.Value.w = boxShadow ( rectangleTopLeft, rectangleBottomRight, point - settings.shadowOffset, settings.sigma, settings.linear );
                        vertexPointer->pos = point;
                        vertexPointer->uv = whiteTexelUV;
                        vertexPointer->col = shadowColor;
                        vertexPointer++;
                    }
                    break;
                }
            }
        }

        ImDrawIdx idx = ( ImDrawIdx ) drawList->_VtxCurrentIdx;

        for ( int r = 0; r < settings.rings - 1; ++r ) {
            const ImDrawIdx startOfRingIndex = idx;
            for ( int i = 0; i < numVerticesInARing - 1; ++i ) {
                drawList->_IdxWritePtr [ 0 ] = idx + 0;
                drawList->_IdxWritePtr [ 1 ] = idx + 1;
                drawList->_IdxWritePtr [ 2 ] = idx + numVerticesInARing;
                drawList->_IdxWritePtr [ 3 ] = idx + 1;
                drawList->_IdxWritePtr [ 4 ] = idx + numVerticesInARing + 1;
                drawList->_IdxWritePtr [ 5 ] = idx + numVerticesInARing;

                idx += 1;
                drawList->_IdxWritePtr += 6;
            }

            drawList->_IdxWritePtr [ 0 ] = idx + 0;
            drawList->_IdxWritePtr [ 1 ] = startOfRingIndex + 0;
            drawList->_IdxWritePtr [ 2 ] = startOfRingIndex + numVerticesInARing;
            drawList->_IdxWritePtr [ 3 ] = idx + 0;
            drawList->_IdxWritePtr [ 4 ] = startOfRingIndex + numVerticesInARing;
            drawList->_IdxWritePtr [ 5 ] = idx + numVerticesInARing;

            drawList->_IdxWritePtr += 6;
            idx += 1;
        }
        drawList->_VtxCurrentIdx += settings.totalVertices;

        if ( settings.enableDebugVisualization ) {
            const ImColor lineColor ( 0, 0, 255, 50 );
            for ( int r = 0; r < settings.rings - 1; ++r ) {
                const ImDrawIdx startOfRingIndex = r * numVerticesInARing;

                ImVec2 a;
                ImVec2 b;
                for ( int i = 0; i < numVerticesInARing - 1; ++i ) {
                    a = shadowVertices [ startOfRingIndex + i + 1 ].pos;
                    b = shadowVertices [ startOfRingIndex + i + numVerticesInARing ].pos;
                    drawList->AddLine ( a, b, lineColor );
                    a = shadowVertices [ startOfRingIndex + i + 0 ].pos;
                    b = shadowVertices [ startOfRingIndex + i + numVerticesInARing ].pos;
                    drawList->AddLine ( a, b, lineColor );
                    a = shadowVertices [ startOfRingIndex + i + numVerticesInARing + 1 ].pos;
                    b = shadowVertices [ startOfRingIndex + i + numVerticesInARing ].pos;
                    drawList->AddLine ( a, b, lineColor );
                }

                a = shadowVertices [ startOfRingIndex + numVerticesInARing - 1 ].pos;
                b = shadowVertices [ startOfRingIndex + numVerticesInARing ].pos;
                drawList->AddLine ( a, b, lineColor );
                a = shadowVertices [ startOfRingIndex + numVerticesInARing + numVerticesInARing - 1 ].pos;
                b = shadowVertices [ startOfRingIndex + numVerticesInARing ].pos;
                drawList->AddLine ( a, b, lineColor );
            }

            for ( int i = 0; i < settings.totalVertices; ++i ) {
                const ImVec2 bmin = shadowVertices [ i ].pos - ImVec2 ( 2, 2 );
                const ImVec2 bmax = shadowVertices [ i ].pos + ImVec2 ( 2, 2 );
                drawList->AddRectFilled ( bmin, bmax, ImColor ( 255, 0, 0, 50 ) );
            }
        }
    }*/

	void child_window::draw ( ) {

	
		//this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 255, 0, 0, 255 ), this->rounding );
		if ( this->type_child == child_type::main_container ) {
			//	&& !static_cast< sub_tab * >( this->tab_assign )->selected && static_cast< tab * >( this->parrent )->selected )
			if ( !static_cast< tab * >( this->tab_assign->parrent )->selected )
				return;

			if ( !static_cast< sub_tab * >( this->tab_assign )->selected )
				return;
		}
		if ( this->type_child == child_type::tab_container ) {
			this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ), this->rounding, ImDrawCornerFlags_::ImDrawCornerFlags_Top );
			this->renderer->AddRectFilled ( this->mins, this->maxs, this->bg_color, this->rounding, ImDrawCornerFlags_::ImDrawCornerFlags_Top );
			this->renderer->AddRectFilled ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ) );

			if ( !this->children.empty ( ) )
				this->renderer->AddRectFilled ( ImVec2 ( this->mins.x, this->children.front ( )->maxs.y ), this->maxs, ImColor ( 50, 50, 50, 255 ) );
		}
		else {
            if ( !( this->flags & flags::no_background ) ) {
                this->renderer->AddRect ( this->mins, this->maxs, ImColor ( 0, 0, 0, 25 ), this->rounding );
                this->renderer->AddRectFilled ( this->mins, this->maxs, SECONDARY_COLOR, this->rounding );
            }
            //addtempshadow ( this->renderer, dsamp );
		}
		

		this->renderer->AddText ( ui::font_title, 14.f, ImVec2 ( this->mins.x + 13, this->mins.y + 13 ), ImColor ( 25, 125, 123, 200 ) , this->title.c_str ( ) );


		for ( size_t i = this->children.size ( ) - 1; i != ( size_t ) -1; i-- )
			this->children.at ( i )->draw ( );

	}
	void child_window::handle ( ) {

	}
	void child_window::handle_mouse_input ( ) {
		auto mouse_pos = ui::get_cursor ( );

		this->hovering = ( mouse_pos.x > this->mins.x && mouse_pos.y > this->mins.y && mouse_pos.x < this->maxs.x && mouse_pos.y < this->maxs.y );
	}
	void child_window::handle_tab_children ( ) {
		this->mins.x = this->parrent->mins.x;
		this->maxs.x = this->parrent->maxs.x;

		this->mins.y = this->parrent->mins.y;
		this->maxs.y = this->mins.y + 90;

	}
	void child_window::update ( ) {

		this->should_reanimate = false;


		if ( this->type_child == child_type::tab_container ) {

			handle_tab_children ( );
			for ( auto & child : this->children )
				child->update ( );

			return;
		}
		if ( this->type_child == child_type::main_container ) {
			this->mins = this->parrent->mins;
			this->mins.y += 90;
			this->maxs = this->parrent->maxs;

			for ( auto & child : this->children )
				child->update ( );
			return;
		}

		if ( this->_float > 0 ) {
			if ( _float == float_side::bottom ) {
				this->mins.x = this->parrent->mins.x;
				this->mins.y = this->parrent->maxs.y;

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			}
		}
		else {
			if ( this->parrent->flex == flex_direction::automatic ) {
				auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					auto & last = this->parrent->children.at ( this->index - 1 );

					this->mins.x = last->mins.x;
					this->mins.y = last->maxs.y + _parrent->padding; ;

				
					this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

					if ( this->maxs.y > this->parrent->maxs.y - 5 && this->tab_assign == nullptr ) {

						/*	auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
							this->mins.y = this->parrent->mins.x + spacing_left;
							this->mins.x = last->maxs.x + _parrent->padding;
							*/

						this->mins.x = last->maxs.x + this->parrent->padding;
						this->mins.y = this->parrent->mins.y + this->parrent->padding;


						this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
					}


				}
				else {

					auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
					this->mins.x = this->parrent->mins.x + spacing_left;
					this->mins.y = this->parrent->mins.y + _parrent->padding;
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

			}
			else if ( this->parrent->flex == flex_direction::block ) {
				auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					try {
						auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins.x = _parrent->mins.x + this->padding;
						this->maxs.x = _parrent->maxs.x - this->padding;


						this->mins.y = back->maxs.y + _parrent->padding;
						this->maxs.y = this->mins.y + this->height;
					}
					catch ( int ex ) {

					}
				}
				else {
					this->mins.x = _parrent->mins.x + this->padding;
					this->maxs.x = _parrent->maxs.x - this->padding;


					this->mins.y = _parrent->mins.y + _parrent->padding;
					this->maxs.y = this->mins.y + this->height;
				}
			}
			else {
				this->mins = this->parrent->mins;

				if ( this->index > 0 ) {
					try {
						auto & back = this->parrent->children.at ( this->index - 1 );
						this->mins.y = back->maxs.y;
					}
					catch ( int ex ) {

					}
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
			}
		}
		for ( auto & child : this->children )
			child->update ( );

	}
}
/*
auto _parrent = static_cast< child_window * >( this->parrent );

				if ( this->index > 0 ) {
					auto & last = this->parrent->children.at ( this->index - 1 );

					this->mins.x = last->maxs.x + _parrent->padding;
					this->mins.y = last->mins.y;

					this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );

					if ( this->maxs.x > this->parrent->maxs.x ) {

						auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
						this->mins.x = this->parrent->mins.x + spacing_left;
						this->mins.y = last->maxs.y + _parrent->padding;


					}


				}
				else {

					auto spacing_left = ( this->parrent->width - ( this->width * 2.f + _parrent->padding ) ) / 2.f;
					this->mins.x = this->parrent->mins.x + spacing_left;
					this->mins.y = this->parrent->mins.y + _parrent->padding;
				}

				this->maxs = ImVec2 ( this->mins.x + this->width, this->mins.y + this->height );
*/